import cv2
import torch
from PIL import Image
from rfdetr.detr import RFDETRBase

model = RFDETRBase(resolution=448)

checkpoint = torch.load("rf-detr-base.pth", map_location="cpu", weights_only=False)

if isinstance(checkpoint, dict) and "model" in checkpoint:
    print("Full checkpoint detected. Extracting weights...")
    state_dict = checkpoint["model"]
else:
    state_dict = checkpoint

model.model.model.load_state_dict(state_dict, strict=True)
model.model.model.eval()

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model.model.model.to(device)

# 3. Start the webcam feed
cap = cv2.VideoCapture("/dev/video34")

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Convert OpenCV BGR format to RGB, then to PIL Image
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    pil_img = Image.fromarray(rgb_frame)

    # 4. Run Inference
    with torch.no_grad():
        results = model.predict(pil_img)

    if isinstance(results, list):
        detections = results[0]
    else:
        detections = results

    # 6. Visualize Detections
    # Now we can safely access attributes on 'detections'
    if hasattr(detections, "xyxy") and detections.xyxy is not None:
        # Iterate through the boxes in this single detections object
        for i in range(len(detections.xyxy)):
            box = detections.xyxy[i]
            conf = detections.confidence
            type = detections.class_id  # HOW TO USE?

            if conf is not None and conf[0] > 0.5:
                # Convert coordinates to integers for OpenCV
                x1, y1, x2, y2 = map(int, box)

                # Draw Box & Label
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(
                    frame,
                    f"{type}: {conf[0]:.2f}",
                    (x1, y1 - 10),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.5,
                    (0, 255, 0),
                    2,
                )

    # Show the video feed
    cv2.imshow("Mobile Phone Detection Live", frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
