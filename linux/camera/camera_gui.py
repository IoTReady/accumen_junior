import sys
from time import sleep
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QLabel, QPushButton, QMessageBox, QFileDialog
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import Qt, QTimer, pyqtSlot
from pypylon import pylon


# GLOABALS
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
        self.__camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice()) 
        self.__img_results = pylon.PylonImage()

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

    def __open_device(self):
        self.__camera.Open()

    def __destroy_all(self):
        try:
            # Release the image and stop grabbing
            if self.__camera.IsGrabbing():
                self.__camera.StopGrabbing()
                sleep(3)        
                self.__camera.Close()
        except Exception as e:
            print(f"Error closing camera: {e}")

    
    def __start_acquisition(self):
        # Check that a device is opened and that the acquisition is NOT running. If not, return.
       
        if self.__camera is None:
            return False

        # Camera Parameters
        # Set exposure time in microseconds
        self.__camera.ExposureTime.SetValue(10000)  # 10,000 microseconds (10 ms)
        
        # Set gain value in dB
        self.__camera.Gain.SetValue(1)  # Set gain to 1 dB

        

        # Setup acquisition timer accordingly

        return True




def dummy():
    pass


if __name__ == "__main__":
    pass
