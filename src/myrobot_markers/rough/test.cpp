#include <iostream>
#include <tf/tf.h>

using namespace std;

#define M_PI    3.14159265358979323846  /* pi */

// Normalise angles between 0 and 2*pi
double normalise_angle(double theta) {
    double theta_norm = std::fmod(theta, 2 * M_PI);
    if (theta_norm < 0) {
        theta_norm += 2 * M_PI;
    }
    return theta_norm;
}

// Decided if 2 poses are close to each other in terms of x, y and theta
// Inputs
//      pose1: [x(m), y(m), theta(rad)]
//      pose2: [x(m), y(m), theta(rad)]
bool position_close(double pose1[3], double pose2[3]) {
    double dist_tolerance = 0.2;
    double angle_tolerance = 1.0;
    double theta1 = normalise_angle(pose1[2]);
    double theta2 = normalise_angle(pose2[2]);

    if (abs(pose1[0] - pose2[0]) < dist_tolerance &&
            abs(pose1[1] - pose2[1]) < dist_tolerance &&
            (M_PI - abs(abs(theta1 - theta2) - M_PI)) < angle_tolerance) {
        return true;
    } else {
        return false;
    }
}

void spiral_goal_position(int pick_id, double spiral_goal[3]) {
    int x, y;
    double m;
    double k = ceil( (sqrt(pick_id) - 1) / 2 );
    double t = 2 * k + 1;
    m = pow(t, 2); 
    t -= 1;
    if (pick_id >= m - t) {
        x = k - ( m - pick_id);
        y = -k;
    } else {
        m = m - t;
    }
    if (pick_id >= m - t) {
        x = -k;
        y = - k + (m - pick_id);
    } else {
        m = m - t;
    }
    if (pick_id >= m - t) {
        x = - k + (m - pick_id);
        y = k;
    } else {
        x = k;
        y = k - (m - pick_id - t);
    }
    double spacing = 0.8;
    spiral_goal[0] = x;  // x-position
    spiral_goal[1] = y;  // y-position
    spiral_goal[2] = 0;  // orientation
}

// https://stackoverflow.com/questions/3706219/algorithm-for-iterating-over-an-outward-spiral-on-a-discrete-2d-grid-from-the-or
class SpiralOut{
protected:
    unsigned layer;
    unsigned leg;
public:
    int x, y; //read these as output from next, do not modify.
    SpiralOut():layer(1),leg(0),x(0),y(0){}
    void goNext(){
        switch(leg){
        case 0: ++x; if(x  == layer)  ++leg;                break;
        case 1: ++y; if(y  == layer)  ++leg;                break;
        case 2: --x; if(-x == layer)  ++leg;                break;
        case 3: --y; if(-y == layer){ leg = 0; ++layer; }   break;
        }
    }
};

int main() {
    // Test quaternion
    double theta = 1.4;
    tf::Quaternion q = tf::Quaternion();
    q.setRPY(0, 0, theta);

    tf::Matrix3x3 R = tf::Matrix3x3(q);  // ROtation matrix
    double roll, pitch, yaw;
    R.getRPY(roll, pitch, yaw);

    cout << "Yaw:" << yaw << "   Theta:" << theta << endl;


    // Test spiral
    double goal[3];
    SpiralOut spiral;
    for (int i = 0; i < 10; i++) {
        // spiral_goal_position(i, goal);
        
        cout << "[" << spiral.x << ", " << spiral.y << "]" << endl;
        spiral.goNext();
        // cout << "[" << goal[0] << ", " << goal[1] << "]" << endl;
    }
}