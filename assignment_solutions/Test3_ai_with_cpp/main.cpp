#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
void swap_faces( Mat image,vector<Rect> &faces,int fir_face,int sec_face ){
     // Check if the face indices are valid
 if (fir_face < 1 || sec_face < 1 || fir_face > faces.size() || sec_face > faces.size()) {
        cout << "Invalid face indices." << endl;
        return;
    }
       // reduce the input to make it 0 based
       fir_face--;
       sec_face--;
       // get the sizes of the selected faces
        cv::Size newSize1(faces[fir_face].size());
        cv::Size newSize2(faces[sec_face].size());
           // create copies of the selected faces
        cv::Mat selectedFace1 = image(faces[fir_face]).clone();
        cv::Mat selectedFace2 = image(faces[sec_face]).clone();

        // resize the first face to be the same height and width of the second face
        cv::resize(selectedFace1, selectedFace1, newSize1);
        cv::resize(selectedFace2, selectedFace2, newSize2);

        // Define the destination region for the resized face
        cv::Rect destRect(faces[sec_face].x, faces[sec_face].y, selectedFace1.cols, selectedFace1.rows);
        cv::Rect destRect2(faces[fir_face].x, faces[fir_face].y, selectedFace2.cols, selectedFace2.rows);
        // Copy the resized face to the destination region

        // copy the first resized face to the second face
        selectedFace1.copyTo(image(destRect));
        // copy the second resized face to the first face
        selectedFace2.copyTo(image(destRect2));

        cv::imshow("Face Detection", image);

}

int main() {
    // Loading the image in which you want to find faces
    Mat image = imread("input.jpg");

    // Check if the image was loaded successfully
    if (image.empty()) {
        cout << "Error: Please reupload the image" << std::endl;
        return -1;
    }

    // Path to the Haar Cascade XML file
    string cascadePath = "haarcascade_frontalface_alt.xml";

    // Load the pre-trained Haar Cascade Classifier for face detection
    cv::CascadeClassifier faceCascade;

    // Check if the classifier was loaded successfully
    if (!faceCascade.load(cascadePath)) {
        cout << "Error: Please reupload Haar Cascade Classifier" << endl;
        return -1;
    }

    // Convert the image to grayscale (required for face detection)
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    // Perform face detection
    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(grayImage, faces, 1.3, 5);

    // Draw rectangles around detected faces
    for (size_t i = 0; i < faces.size(); ++i) {
        cv::Rect face = faces[i];
        cv::rectangle(image, face, cv::Scalar(0, 255, 0), 2); // Green rectangle
        std::string faceNumber = "Face " + std::to_string(i + 1);

        // Calculate the position for the label
        int x = face.x;
        int y = face.y - 10; // Shift slightly above the face rectangle

        // Add the label to the image
        cv::putText(image, faceNumber, cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
    }

    // Check if there are at least two detected faces
    if (faces.size() >= 2) {
     swap_faces(image,faces,2,3);
    // swap_faces(image,faces,3,2);
    // swap_faces(image,faces,1,4);
    }
      cv::imwrite("output.jpg",image);
  //  cv::imshow("Face Detection", image);
    cv::waitKey(0);

    return 0;
}
