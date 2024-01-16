import sys
import time
import numpy as np
from pypylon import pylon
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QPushButton, QVBoxLayout, QWidget
from PyQt5.QtGui import QImage, QPixmap, QPainter
from PyQt5.QtCore import Qt, QTimer

class CameraApp(QMainWindow):
    def __init__(self):
        super().__init__()

        self.initUI()

        self.camera, self.img_res = self.init_camera()
        if self.camera:
            self.timer = QTimer(self)
            self.timer.timeout.connect(self.update_preview)
            self.timer.start(100)

    def initUI(self):
        self.setWindowTitle("Camera App")
        self.setGeometry(100, 100, 800, 600)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)

        self.preview_label = QLabel(self)
        self.capture_button = QPushButton("Capture", self)
        self.capture_button.clicked.connect(self.capture_frame)

        layout = QVBoxLayout()
        layout.addWidget(self.preview_label)
        layout.addWidget(self.capture_button)
        self.central_widget.setLayout(layout)

    def init_camera(self):
        try:
            camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())
            img_res = pylon.PylonImage()
            camera.Open()
            camera.ExposureTime.SetValue(10000)  # 10,000 microseconds (10 ms)
            camera.PixelFormat.SetValue("RGB8")
            camera.Gain.SetValue(1)  # Set gain to 6 dB
            return camera, img_res
        except Exception as e:
            print(f"Error initializing camera: {e}")
            return None, None

    def capture_frame(self):
        img = self.capture_frame_from_camera()
        if img:
            self.save_image(img)
            print("sv File ")

    def capture_frame_from_camera(self):
        try:
            if self.camera.IsGrabbing():
                self.camera.StopGrabbing()
            self.camera.StartGrabbing(pylon.GrabStrategy_LatestImageOnly)
            img = self.camera.RetrieveResult(10000, pylon.TimeoutHandling_ThrowException)
            return img
        except Exception as e:
            print(f"Error capturing frame: {e}")
            return None

    def save_image(self, img):
        try:
            img_res = self.img_res 
            img_res.AttachGrabResultBuffer(img)
            print("saved image")
            timestr = time.strftime("%Y%m%d-%H%M%S")
            filename = f"{timestr}.png"
            img_res.Save(pylon.ImageFileFormat_Tiff, filename)
            print(f"Image saved as {filename}")
            img.Release()
        except Exception as e:
            print(f"Error saving image: {e}")

    def update_preview(self):
        img = self.capture_frame_from_camera()
        if img:
            frame = np.ascontiguousarray(img.GetArray())
            qframe = QImage(
                frame.data,
                frame.shape[1],
                frame.shape[0],
                frame.shape[1]*3,
                QImage.Format_RGB888
            )
            pixmap = QPixmap.fromImage(qframe)
            resized_pixmap = pixmap.scaled(1280, 720, Qt.KeepAspectRatio)
            self.preview_label.setPixmap(resized_pixmap)

    def closeEvent(self, event):
        if self.camera.IsGrabbing():
            self.camera.StopGrabbing()
        self.camera.Close()

def main():
    app = QApplication(sys.argv)
    window = CameraApp()
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()

