import sys
import cv2
from ids_peak import ids_peak, ids_peak_ipl_extension
from ids_peak_ipl import ids_peak_ipl

VERSION = "1.2.0"
FPS_LIMIT = 30
EXPOSURE = 100000
GAIN = 1.0
X_OFFSET = 0
Y_OFFSET = 0
WIDTH = 4000
HEIGHT = 3000
ExposureAuto = "Continuous"
GainAuto = "Continuous"

class CameraCLI:
    def __init__(self):
        self.filename = 0
        self.displayed_image = None

        self.__device = None
        self.__nodemap_remote_device = None
        self.__datastream = None

        self.__acquisition_timer = cv2.TickMeter()
        self.__frame_counter = 0
        self.__error_counter = 0
        self.__acquisition_running = False

        ids_peak.Library.Initialize()

        if self.__open_device():
            try:
                if not self.__start_acquisition():
                    print("Unable to start acquisition!")
                    sys.exit(1)
            except Exception as e:
                print(f"Exception: {str(e)}")
                sys.exit(1)

        else:
            self.__destroy_all()
            sys.exit(1)

    def __del__(self):
        self.__destroy_all()

    def __destroy_all(self):
        self.__stop_acquisition()
        self.__close_device()
        ids_peak.Library.Close()

    def __open_device(self):
        try:
            device_manager = ids_peak.DeviceManager.Instance()
            device_manager.Update()

            if device_manager.Devices().empty():
                print("No device found!")
                return False

            for device in device_manager.Devices():
                if device.IsOpenable():
                    self.__device = device.OpenDevice(ids_peak.DeviceAccessType_Control)
                    break

            if self.__device is None:
                print("Device could not be opened!")
                return False

            datastreams = self.__device.DataStreams()
            if datastreams.empty():
                print("Device has no DataStream!")
                self.__device = None
                return False

            self.__datastream = datastreams[0].OpenDataStream()

            self.__nodemap_remote_device = self.__device.RemoteDevice().NodeMaps()[0]

            try:
                self.__nodemap_remote_device.FindNode("UserSetSelector").SetCurrentEntry("Default")
                self.__nodemap_remote_device.FindNode("UserSetLoad").Execute()
                self.__nodemap_remote_device.FindNode("UserSetLoad").WaitUntilDone()
            except ids_peak.Exception:
                pass

            payload_size = self.__nodemap_remote_device.FindNode("PayloadSize").Value()
            buffer_count_max = self.__datastream.NumBuffersAnnouncedMinRequired()

            for i in range(buffer_count_max):
                buffer = self.__datastream.AllocAndAnnounceBuffer(payload_size)
                self.__datastream.QueueBuffer(buffer)

            return True
        except ids_peak.Exception as e:
            print(f"Exception: {str(e)}")

        return False

    def __close_device(self):
        self.__stop_acquisition()

        if self.__datastream is not None:
            try:
                for buffer in self.__datastream.AnnouncedBuffers():
                    self.__datastream.RevokeBuffer(buffer)
            except Exception as e:
                print(f"Exception: {str(e)}")

    def __start_acquisition(self):
        if self.__device is None:
            return False
        if self.__acquisition_running is True:
            return True

        try:
            max_fps = self.__nodemap_remote_device.FindNode("AcquisitionFrameRate").Maximum()
            target_fps = int(min(max_fps, FPS_LIMIT))
            self.__nodemap_remote_device.FindNode("AcquisitionFrameRate").SetValue(target_fps)
        except ids_peak.Exception:
            print("Warning: Unable to limit fps, AcquisitionFrameRate Node is not supported by the connected camera.")

        self.__nodemap_remote_device.FindNode("ExposureTime").SetValue(EXPOSURE)
        self.__nodemap_remote_device.FindNode("GainAuto").SetCurrentEntry(GainAuto)
        self.__nodemap_remote_device.FindNode("BalanceWhiteAuto").SetCurrentEntry("Continuous")

        self.__acquisition_timer.start()
        self.__acquisition_timer.reset()

        try:
            self.__nodemap_remote_device.FindNode("TLParamsLocked").SetValue(1)
            self.__datastream.StartAcquisition()
            self.__nodemap_remote_device.FindNode("AcquisitionStart").Execute()
            self.__nodemap_remote_device.FindNode("AcquisitionStart").WaitUntilDone()
        except Exception as e:
            print(f"Exception: {str(e)}")
            return False

        self.__acquisition_running = True

        return True

    def __stop_acquisition(self):
        if self.__device is None or self.__acquisition_running is False:
            return

        try:
            remote_nodemap = self.__device.RemoteDevice().NodeMaps()[0]
            remote_nodemap.FindNode("AcquisitionStop").Execute()
            self.__datastream.KillWait()
            self.__datastream.StopAcquisition(ids_peak.AcquisitionStopMode_Default)
            self.__datastream.Flush(ids_peak.DataStreamFlushMode_DiscardAll)
            self.__acquisition_running = False

            if self.__nodemap_remote_device is not None:
                try:
                    self.__nodemap_remote_device.FindNode("TLParamsLocked").SetValue(0)
                except Exception as e:
                    print(f"Exception: {str(e)}")

        except Exception as e:
            print(f"Exception: {str(e)}")

    def capture_image(self):
        try:
            if self.displayed_image:
                image_path = f"pd_{self.filename}.jpg"
                self.displayed_image.save(image_path, "JPEG")
                self.filename += 1
                print(f"Image saved as {image_path}.")
        except Exception as e:
            print(f"Error: {str(e)}")

    def run(self):
        while True:
            try:
                buffer = self.__datastream.WaitForFinishedBuffer(5000)
                ipl_image = ids_peak_ipl_extension.BufferToImage(buffer)
                converted_ipl_image = ipl_image.ConvertTo(ids_peak_ipl.PixelFormatName_BGRa8)
                self.__datastream.QueueBuffer(buffer)

                image_np_array = converted_ipl_image.get_numpy_1D()
                image = cv2.cvtColor(image_np_array, cv2.COLOR_BGR2RGB)
                self.displayed_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

                self.__frame_counter += 1
                self.update_counters()
            except ids_peak.Exception as e:
                self.__error_counter += 1
                print(f"Exception: {str(e)}")

    def update_counters(self):
        print(f"Acquired: {self.__frame_counter}, Errors: {self.__error_counter}")

def main():
    camera_cli = CameraCLI()
    camera_cli.run()

if __name__ == "__main__":
    main()

