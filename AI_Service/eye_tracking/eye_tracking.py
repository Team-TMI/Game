# eye_tracking.py
import cv2
import mediapipe as mp
import numpy as np
import pyautogui
import win32file
from utils.packet_utils import pack_eye_tracking_response_with_header, get_coordinate_pipe

SEND_EVERY_N_FRAMES = 2
PIPE_SEND = r"\\.\pipe\python_to_unreal"  # Python → Unreal 좌표 전송용 파이프

def run_relative_eye_tracking(screen_w, screen_h, stop_event=None):
    cap = cv2.VideoCapture(0)
    face_mesh = mp.solutions.face_mesh.FaceMesh(refine_landmarks=True)

    prev_eye_x, prev_eye_y = None, None
    cursor_x, cursor_y = screen_w // 2, screen_h // 2  # 화면 중앙에서 시작

    print("🎯 상대 좌표 기반 시선 추적 시작")

    pipe = get_coordinate_pipe()
    if pipe is None or pipe == win32file.INVALID_HANDLE_VALUE:
        print("❌ 좌표 전송용 파이프가 아직 연결되지 않았습니다.")
        return
    print("📡 좌표 전송용 파이프 핸들 획득 완료")

    frame_count = 0
    distance_points = [0.09, 0.15, 0.2]
    scale_points = [150000, 80000, 60000]

    while stop_event is None or not stop_event.is_set():
        ret, frame = cap.read()
        if not ret:
            break

        frame = cv2.flip(frame, 1)
        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        result = face_mesh.process(rgb)

        if result.multi_face_landmarks:
            face = result.multi_face_landmarks[0].landmark
            left_iris = face[468]
            right_iris = face[473]
            eye_x = (left_iris.x + right_iris.x) / 2
            eye_y = (left_iris.y + right_iris.y) / 2

            left_eye_outer = face[33]
            right_eye_outer = face[263]
            current_eye_distance = np.linalg.norm([
                left_eye_outer.x - right_eye_outer.x,
                left_eye_outer.y - right_eye_outer.y
            ])
            
            scale = float(np.interp(current_eye_distance, distance_points, scale_points))

            if prev_eye_x is not None:
                dx = eye_x - prev_eye_x
                dy = eye_y - prev_eye_y

                cursor_x += int(dx * scale)
                cursor_y += int(dy * scale)

                gaze_x = np.clip(cursor_x, 0, screen_w - 1)
                gaze_y = np.clip(cursor_y, 0, screen_h - 1)

                frame_count += 1
                if frame_count % SEND_EVERY_N_FRAMES == 0:
                    try:
                        packet = pack_eye_tracking_response_with_header(
                            quiz_id=1,
                            x=float(gaze_x),
                            y=float(gaze_y),
                            blink=0,
                            state=100
                        )
                        win32file.WriteFile(pipe, packet)
                        print(f"📤 좌표 전송: ({gaze_x}, {gaze_y}) | 거리: {current_eye_distance:.4f} | 스케일: {scale:.0f}")
                    except Exception as e:
                        print(f"❌ 좌표 전송 실패: {e}")
                        break

            prev_eye_x, prev_eye_y = eye_x, eye_y

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    # win32file.CloseHandle(pipe)
    print("🛑 시선 추적 종료")
