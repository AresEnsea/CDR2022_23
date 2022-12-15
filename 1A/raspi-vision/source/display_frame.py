import numpy as np
import cv2
from camera_info import *
from random import randint

class FakeDisplay:
    def __init__(self) -> None:
        pass

    def show(self, *args):
        return True
    
    def release(self):
        pass

class RealDisplay:
    def __init__(self) -> None:
        # cv2.namedWindow('extra', cv2.WINDOW_NORMAL)
        # cv2.resizeWindow('extra', CameraConstants.SensorW*3//4, CameraConstants.SensorH*3//4)
        cv2.namedWindow('aru_img', cv2.WINDOW_NORMAL)
        cv2.resizeWindow('aru_img', int(CameraConstants.SensorW * CameraConstants.Xscale), int(CameraConstants.SensorH * CameraConstants.Yscale))
        cv2.setMouseCallback('aru_img', self.spawn_cake_part)
        self.have_new_data = False

    def show(self, frame_source, img):
        cv2.imshow('aru_img', img)
        k = cv2.waitKey(2) & 0xff
        if k == 27:
            return False
        elif frame_source.is_fake:
            if self.have_new_data:
                click_pos = frame_source.get_world_space_proj(self.click_x, self.click_y)
                if click_pos is not None:
                    frame_source.add_data(self.to_append_kind, self.to_append_angle, click_pos)
                self.have_new_data = False

            if k == ord('s'):
                cam_pitch = frame_source.cam_pitch
                cam_pitch = max(cam_pitch - frame_source.cam_ang_speed, frame_source.cam_ang_bounds[0])
                if cam_pitch != frame_source.cam_pitch:
                    frame_source.cam_pitch = cam_pitch
                    frame_source.cam_update = True
            elif k == ord('z'):
                cam_pitch = frame_source.cam_pitch
                cam_pitch = min(cam_pitch + frame_source.cam_ang_speed, frame_source.cam_ang_bounds[1])
                if cam_pitch != frame_source.cam_pitch:
                    frame_source.cam_pitch = cam_pitch
                    frame_source.cam_update = True
            elif k == ord('q'):
                if np.all(frame_source.cam_pos * frame_source.cam_dir == frame_source.cam_bound_lower):
                    frame_source.next_cam_bound(-1)
                else:
                    frame_source.cam_pos -= frame_source.cam_speed * frame_source.cam_dir
                    frame_source.cam_update = True
            elif k == ord('d'):
                if np.all(frame_source.cam_pos * frame_source.cam_dir == frame_source.cam_bound_upper):
                    frame_source.next_cam_bound(+1)
                else:
                    frame_source.cam_pos += frame_source.cam_speed * frame_source.cam_dir
                    frame_source.cam_update = True
            elif k == ord('u'):
                cam_h = frame_source.cam_pos[1]
                if cam_h != min(cam_h + frame_source.cam_speed, CameraInfo.ElevationLimites[1]):
                    frame_source.cam_pos[1] = cam_h + frame_source.cam_speed
                    frame_source.cam_update = True
            elif k == ord('j'):
                cam_h = frame_source.cam_pos[1]
                if cam_h != max(cam_h - frame_source.cam_speed, CameraInfo.ElevationLimites[0]):
                    frame_source.cam_pos[1] = cam_h - frame_source.cam_speed
                    frame_source.cam_update = True
            elif k == ord('i'):
                print("camera info:")
                print("Angle vert:", CameraInfo.AngleVertical, "hori:", CameraInfo.AngleHorizontal)
                print("Position Lateral", CameraInfo.Lateral, "hauteur:", CameraInfo.Elevation, "profondeur:", CameraInfo.Profondeur)
        return True

    def release(self):
        cv2.destroyAllWindows()
    
    def spawn_cake_part(self, event, x, y, flags, param):
        if event == cv2.EVENT_LBUTTONDOWN:
            self.click_x = x
            self.click_y = y
            self.to_append_kind = randint(0,2)
            self.to_append_angle = randint(-180, 180)
            self.have_new_data = True
