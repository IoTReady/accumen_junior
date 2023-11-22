import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QLabel, QPushButton, QMessageBox, QFileDialog
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import Qt, QTimer, pyqtSlot
from ids_peak import ids_peak
from ids_peak_ipl import ids_peak_ipl
from ids_peak import ids_peak_ipl_extension

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

class MainWindow(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("IDS Camera App")
        self.setGeometry(100, 100, 800, 600)

        self.widget = QWidget(self)
        self.__layout = QVBoxLayout()
        self.widget.setLayout(self.__layout)
        self.setCentralWidget(self.widget)

        # self.layout = QVBoxLayout(self.centralWidget())

        self.image_label = QLabel(self)
        # self.image_label.setFixedSize(2048,1440)
        self.__layout.addWidget(self.image_label)

        self.capture_button = QPushButton("Capture", self)
        self.capture_button.clicked.connect(self.capture_image)
        self.__layout.addWidget(self.capture_button)

        # self.exit_button = QPushButton("Exit", self)
        # self.exit_button.clicked.connect(self.__del__)
        # self.__layout.addWidget(self.exit_button)

        self.filename = 0
        self.displayed_image = None

        self.__device = None
        self.__nodemap_remote_device = None
        self.__datastream = None

        self.__display = None
        self.__acquisition_timer = QTimer()
        self.__frame_counter = 0
        self.__error_counter = 0
        self.__acquisition_running = False

        self.__label_infos = None
        self.__label_version = None
        self.__label_aboutqt = None

        # Initialize peak library
        ids_peak.Library.Initialize()

        if self.__open_device():
            try:
                # Create a display for the camera image
                self.__display = QLabel()
                self.__display.setFixedSize(500,500)
                self.__layout.addWidget(self.__display)
                if not self.__start_acquisition():
                    QMessageBox.critical(self, "Unable to start acquisition!", QMessageBox.Ok)
            except Exception as e:
                QMessageBox.critical(self, "Exception", str(e), QMessageBox.Ok)

        else:
            self.__destroy_all()
            sys.exit(0)

        self.__create_statusbar()

        self.setMinimumSize(700, 500)

    def __del__(self):
        self.__destroy_all()

    def __destroy_all(self):
        # Stop acquisition
        self.__stop_acquisition()

        # Close device and peak library
        self.__close_device()
        ids_peak.Library.Close()

    def __open_device(self):
        try:
            # Create instance of the device manager
            device_manager = ids_peak.DeviceManager.Instance()

            # Update the device manager
            device_manager.Update()

            # Return if no device was found
            if device_manager.Devices().empty():
                QMessageBox.critical(self, "Error", "No device found!", QMessageBox.Ok)
                return False

            # Open the first openable device in the managers device list
            for device in device_manager.Devices():
                if device.IsOpenable():
                    self.__device = device.OpenDevice(ids_peak.DeviceAccessType_Control)
                    break

            # Return if no device could be opened
            if self.__device is None:
                QMessageBox.critical(self, "Error", "Device could not be opened!", QMessageBox.Ok)
                return False

            # Open standard data stream
            datastreams = self.__device.DataStreams()
            if datastreams.empty():
                QMessageBox.critical(self, "Error", "Device has no DataStream!", QMessageBox.Ok)
                self.__device = None
                return False

            self.__datastream = datastreams[0].OpenDataStream()

            # Get nodemap of the remote device for all accesses to the GenICam nodemap tree
            self.__nodemap_remote_device = self.__device.RemoteDevice().NodeMaps()[0]

            # To prepare for untriggered continuous image acquisition, load the default user set if available and
            # wait until execution is finished
            try:
                self.__nodemap_remote_device.FindNode("UserSetSelector").SetCurrentEntry("Default")
                self.__nodemap_remote_device.FindNode("UserSetLoad").Execute()
                self.__nodemap_remote_device.FindNode("UserSetLoad").WaitUntilDone()
            except ids_peak.Exception:
                # Userset is not available
                pass

            # Get the payload size for correct buffer allocation
            payload_size = self.__nodemap_remote_device.FindNode("PayloadSize").Value()

            # Get minimum number of buffers that must be announced
            buffer_count_max = self.__datastream.NumBuffersAnnouncedMinRequired()

            # Allocate and announce image buffers and queue them
            for i in range(buffer_count_max):
                buffer = self.__datastream.AllocAndAnnounceBuffer(payload_size)
                self.__datastream.QueueBuffer(buffer)

            return True
        except ids_peak.Exception as e:
            QMessageBox.critical(self, "Exception", str(e), QMessageBox.Ok)

        return False

    def __close_device(self):
        """
        Stop acquisition if still running and close datastream and nodemap of the device
        """
        # Stop Acquisition in case it is still running
        self.__stop_acquisition()

        # If a datastream has been opened, try to revoke its image buffers
        if self.__datastream is not None:
            try:
                for buffer in self.__datastream.AnnouncedBuffers():
                    self.__datastream.RevokeBuffer(buffer)
            except Exception as e:
                QMessageBox.information(self, "Exception", str(e), QMessageBox.Ok)

    def __start_acquisition(self):
        """
        Start Acquisition on camera and start the acquisition timer to receive and display images

        :return: True/False if acquisition start was successful
        """
        # Check that a device is opened and that the acquisition is NOT running. If not, return.
       
        if self.__device is None:
            return False
        if self.__acquisition_running is True:
            return True

        # Get the maximum framerate possible, limit it to the configured FPS_LIMIT. If the limit can't be reached, set
        # acquisition interval to the maximum possible framerate
        try:
            max_fps = self.__nodemap_remote_device.FindNode("AcquisitionFrameRate").Maximum()
            target_fps = int(min(max_fps, FPS_LIMIT))
            self.__nodemap_remote_device.FindNode("AcquisitionFrameRate").SetValue(target_fps)
        except ids_peak.Exception:
            # AcquisitionFrameRate is not available. Unable to limit fps. Print warning and continue on.
            QMessageBox.warning(self, "Warning",
                                "Unable to limit fps, since the AcquisitionFrameRate Node is"
                                " not supported by the connected camera. Program will continue without limit.")

        # Camera Parameters
        self.__nodemap_remote_device.FindNode("ExposureTime").SetValue(EXPOSURE)
        # self.__nodemap_remote_device.FindNode("Gain").SetValue(GAIN)
        self.__nodemap_remote_device.FindNode("OffsetX").SetValue(X_OFFSET)
        self.__nodemap_remote_device.FindNode("OffsetY").SetValue(Y_OFFSET)
        self.__nodemap_remote_device.FindNode("Width").SetValue(WIDTH)
        self.__nodemap_remote_device.FindNode("Height").SetValue(HEIGHT)
        # self.__nodemap_remote_device.FindNode("ExposureAuto").SetCurrentEntry(ExposureAuto)
        self.__nodemap_remote_device.FindNode("GainAuto").SetCurrentEntry(GainAuto)
        self.__nodemap_remote_device.FindNode("BalanceWhiteAuto").SetCurrentEntry("Continuous")

        

        # Setup acquisition timer accordingly
        print(f"INterva:  {int((1 / target_fps) * 1000)}")
        self.__acquisition_timer.setInterval(int((1 / target_fps) * 1000))
        self.__acquisition_timer.setSingleShot(False)
        self.__acquisition_timer.timeout.connect(self.on_acquisition_timer)

        try:
            # Lock critical features to prevent them from changing during acquisition
            self.__nodemap_remote_device.FindNode("TLParamsLocked").SetValue(1)

            # Start acquisition on camera
            self.__datastream.StartAcquisition()
            self.__nodemap_remote_device.FindNode("AcquisitionStart").Execute()
            self.__nodemap_remote_device.FindNode("AcquisitionStart").WaitUntilDone()
        except Exception as e:
            print("Exception: " + str(e))
            return False

        # Start acquisition timer
        self.__acquisition_timer.start()
        self.__acquisition_running = True

        return True

    def __stop_acquisition(self):
        """
        Stop acquisition timer and stop acquisition on camera
        :return:
        """
        # Check that a device is opened and that the acquisition is running. If not, return.
        if self.__device is None or self.__acquisition_running is False:
            return

        # Otherwise try to stop acquisition
        try:
            remote_nodemap = self.__device.RemoteDevice().NodeMaps()[0]
            remote_nodemap.FindNode("AcquisitionStop").Execute()

            # Stop and flush datastream
            self.__datastream.KillWait()
            self.__datastream.StopAcquisition(ids_peak.AcquisitionStopMode_Default)
            self.__datastream.Flush(ids_peak.DataStreamFlushMode_DiscardAll)

            self.__acquisition_running = False

            # Unlock parameters after acquisition stop
            if self.__nodemap_remote_device is not None:
                try:
                    self.__nodemap_remote_device.FindNode("TLParamsLocked").SetValue(0)
                except Exception as e:
                    QMessageBox.information(self, "Exception", str(e), QMessageBox.Ok)

        except Exception as e:
            QMessageBox.information(self, "Exception", str(e), QMessageBox.Ok)

    def __create_statusbar(self):
        status_bar = QWidget(self.centralWidget())
        status_bar_layout = QVBoxLayout()
        status_bar_layout.setContentsMargins(0, 0, 0, 0)

        self.__label_infos = QLabel(status_bar)
        self.__label_infos.setAlignment(Qt.AlignLeft)
        status_bar_layout.addWidget(self.__label_infos)
        status_bar_layout.addStretch()

        self.__label_version = QLabel(status_bar)
        self.__label_version.setText("simple_live_qtwidgets v" + VERSION)
        self.__label_version.setAlignment(Qt.AlignRight)
        status_bar_layout.addWidget(self.__label_version)

        self.__label_aboutqt = QLabel(status_bar)
        self.__label_aboutqt.setObjectName("aboutQt")
        self.__label_aboutqt.setText("<a href='#aboutQt'>About Qt</a>")
        self.__label_aboutqt.setAlignment(Qt.AlignRight)
        self.__label_aboutqt.linkActivated.connect(self.on_aboutqt_link_activated)
        status_bar_layout.addWidget(self.__label_aboutqt)
        status_bar.setLayout(status_bar_layout)

        self.__layout.addWidget(status_bar)

    def update_counters(self):
        """
        This function gets called when the frame and error counters have changed
        :return:
        """
        self.__label_infos.setText("Acquired: " + str(self.__frame_counter) + ", Errors: " + str(self.__error_counter))

    @pyqtSlot()
    def on_acquisition_timer(self):
        """
        This function gets called on every timeout of the acquisition timer
        """
        try:
            # Get buffer from device's datastream
            buffer = self.__datastream.WaitForFinishedBuffer(5000)

            # Create IDS peak IPL image for debayering and convert it to RGBa8 format
            ipl_image = ids_peak_ipl_extension.BufferToImage(buffer)
            converted_ipl_image = ipl_image.ConvertTo(ids_peak_ipl.PixelFormatName_BGRa8)

            # Queue buffer so that it can be used again
            self.__datastream.QueueBuffer(buffer)

            # Get raw image data from converted image and construct a QImage from it
            image_np_array = converted_ipl_image.get_numpy_1D()
            image = QImage(image_np_array,
                           converted_ipl_image.Width(), converted_ipl_image.Height(),
                           QImage.Format_RGB32)

            
            # Make an extra copy of the QImage to make sure that memory is copied and can't get overwritten later on
            image_cpy = image.copy()

            self.displayed_image = image_cpy
            # self.__display.on_image_received(image_cpy)
            # self.__display.setPixmap(image_cpy)

            # Display the image
            pixmap = QPixmap.fromImage(image)
            # Scale the QPixmap to fit within the QLabel without cropping
            scaled_pixmap = pixmap.scaled(self.__display.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation)
            # Set the scaled QPixmap in the QLabel
            self.__display.setPixmap(scaled_pixmap)
            
            # self.__display.setPixmap(QPixmap.fromImage(image_cpy))
            self.__display.update()

            # Increase frame counter
            self.__frame_counter += 1
        except ids_peak.Exception as e:
            self.__error_counter += 1
            print("Exception: " + str(e))

        # Update counters
        self.update_counters()

    @pyqtSlot(str)
    def on_aboutqt_link_activated(self, link):
        if link == "#aboutQt":
            QMessageBox.aboutQt(self, "About Qt")

    @pyqtSlot()
    def capture_image(self):
        print(self.displayed_image)
        try:
            # if self.displayed_image:
            #     options = QFileDialog.Options()
            #     file_path, _ = QFileDialog.getSaveFileName(self, "Save Image", "", "JPEG Files (*.jpg);;All Files (*)", options=options)
                
            #     if file_path:
            #         self.displayed_image.save(file_path, "JPEG")
            #         QMessageBox.information(self, "Image Saved", "Image saved as JPEG.")
            if self.displayed_image:
                    print("Got the image")
                    self.displayed_image.save(f"pd_{self.filename}.jpg", "JPEG")
                    self.filename += 1
                    QMessageBox.information(self, "Image Saved", f"Image saved as pd_{self.filename}.jpg .")
        except Exception as e:
            QMessageBox.critical(self, "Error", str(e), QMessageBox.Ok)


def main():
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()

