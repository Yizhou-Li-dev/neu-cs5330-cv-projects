//
//  vidDisplay.cpp
//  project1
//
//  Created by Yizhou Li on 12/25/22.
//

#include "vidDisplay.hpp"
#include "obj_rec_methods.hpp"
#include "csv_util.hpp"
#include "general_utils.hpp"
#include <algorithm>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <filesystem>
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;
using namespace cv;
using namespace filesystem;
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

void startVid() {
    VideoCapture cap(0); // 0 for Iphone camer, and 1 for built in Mac webcam
    if(!cap.isOpened()) {
        cout << "Unable to open video device" << endl;
        return;
    }
    
    namedWindow("Live Video");
    Mat src;
    Mat dst;
    char lastKey = 's';
    
    // Creating a directory
    if (mkdir("./saved_images", 0777) == -1)
        cerr << "Error :  " << strerror(errno) << endl;
    else
        cout << "Directory created";
    
    while (true) {
        cap >> src;
        if(src.empty()) {
            printf("src is empty\n");
            break;
        }
        
        // see if there is a waiting keystroke
        char key = waitKey(25);
        if (key == -1) {
            key = lastKey;
        }
        
        dst = src.clone();
        if (key == 'q') {
            break;
        } else if (key == 's') {
            lastKey = 's';
        } else if (key == '1') {
            threshold_image(src, dst);
            imwrite("./saved_images/task1.png", dst);
            lastKey = '1';
        } else if (key == '2') {
            threshold_image(src, dst);
            src = dst.clone();
            custom_morphological_filtering(src, dst);
            imwrite("./saved_images/task2.png", dst);
            lastKey = '2';
        } else if (key == '3') {
            cout << "Type the largest regions you want to display. Alternatively, type 0 to display all regions" << endl;
            int n;
            cin >> n;
            
            while (true) {
                threshold_image(src, dst);
                src = dst.clone();
                
                custom_morphological_filtering(src, dst);
                src = dst.clone();
                
                Mat stats;
                connected_components_analysis(dst, stats, n);
                imwrite("./saved_images/task3.png", dst);
                imshow("Live Video", dst);
                key = waitKey(25);
                if (key != -1) {
                    lastKey = key;
                    break;
                }
                cap >> src;
            }
        } else if (key == '4') {
            cout << "Type the largest regions you want to display. Alternatively, type 0 to display all regions"  << endl;
            int n;
            cin >> n;
            
            threshold_image(src, dst);
            src = dst.clone();
            
            custom_morphological_filtering(src, dst);
            src = dst.clone();
            
            Mat stats;
            int label_size = connected_components_analysis(src, stats, n);
            imshow("Live Video", src);
            
            cout << "Press any key to continue" << endl;
            waitKey(0);
            
            cout << "Choose the region number you want to compute features for" << endl;
            int number;
            cin >> number;
            
            while (true) {
                vector<double> features;
                get_features_from_a_region(src, number, label_size, stats, features);
                
                imwrite("./saved_images/task4.png", src);
                imshow("Live Video", src);
                key = waitKey(25);
                if (key != -1) {
                    lastKey = key;
                    break;
                }
                cap >> src;
                threshold_image(src, dst);
                src = dst.clone();
                
                custom_morphological_filtering(src, dst);
                src = dst.clone();
                
                label_size = connected_components_analysis(src, stats, n);
            }
        } else if (key == '5') {
            threshold_image(src, dst);
            src = dst.clone();
            custom_morphological_filtering(src, dst);
            src = dst.clone();
            
            cout << "Type the largest regions you want to display. Alternat0ively, type 0 to display all regions" << endl;
            int n;
            cin >> n;
            
            Mat stats;
            int label_size = connected_components_analysis(dst, stats, n);
            
            imshow("Live Video", dst);
            
            cout << "Press any key to continue" << endl;
            waitKey(0);
            cout << "Choose the region number you want to compute features for" << endl;
            int number;
            cin >> number;
            
            vector<double> features;
            get_features_from_a_region(src, number, label_size, stats, features);
            
            cout << "please enter the label for your selected region ID" << endl;
            string label;
            cin >> label;
            
            write_csv("./db.csv", {label, features});
            lastKey = 's';
        } else if (key == '6') {
            // ask which object does user wants to compute features for
            threshold_image(src, dst);
            src = dst.clone();
            custom_morphological_filtering(src, dst);
            src = dst.clone();
            
            cout << "Type the largest regions you want to display. Alternatively, type 0 to display all regions" << endl;
            int n;
            cin >> n;
            
            Mat stats;
            int label_size = connected_components_analysis(dst, stats, n);
            
            imshow("Live Video", dst);
            cout << "Press any key to continue" << endl;
            waitKey(0);
            cout << "Choose the region number you want to compute features for" << endl;
            int number;
            cin >> number;
            
            vector<double> the_obj_features;
            get_features_from_a_region(src, number, label_size, stats, the_obj_features);
            
            // read in feature vectors
            vector<pair<string, vector<double>>> rst;
            read_csv_bylines("./db.csv", rst);
            
            // calculate standard deviation for each feature
            double num_of_features = rst[0].second.size();
            vector<vector<double>> std_features(num_of_features, vector<double>(rst.size(), 0));
            
            for (int i = 0; i < rst.size(); i++) {
                vector<double> tmp_vec = rst[i].second;
                for (int j = 0; j < tmp_vec.size(); j++) {
                    std_features[j][i] = tmp_vec[j];
                }
            }
            
            vector<double> stds(num_of_features, 0);
            for (int i = 0; i < std_features.size(); i++) {
                stds[i] =  StandardDeviation(std_features[i]);
            }
            
            // loop rst and calculate the closest label
            map<double, string> the_map;
            for (int i = 0; i < rst.size(); i++) {
                vector<double> tmp_vec = rst[i].second;
                string label = rst[i].first;
                double distance = scaled_euclidian_distance(tmp_vec, the_obj_features, stds);
                the_map.insert({distance, label});
            }
            cout << "The predicted label of your selected object is " + the_map.begin() -> second << endl;
            
            lastKey = 's';
        } else if (key == '7') {
            // ask which object does user wants to compute features for
            threshold_image(src, dst);
            src = dst.clone();
            custom_morphological_filtering(src, dst);
            src = dst.clone();
            
            cout << "Type the largest regions you want to display. Alternatively, type 0 to display all regions" << endl;
            int n;
            cin >> n;
            
            Mat stats;
            int label_size = connected_components_analysis(dst, stats, n);
            
            imshow("Live Video", dst);
            cout << "Press any key to continue" << endl;
            waitKey(0);
            cout << "Choose the region number you want to compute features for" << endl;
            int number;
            cin >> number;
            cout << "Choose the k for k nearest neighbors" << endl;
            int k;
            cin >> k;
            
            vector<double> the_obj_features;
            get_features_from_a_region(src, number, label_size, stats, the_obj_features);
            
            // read in feature vectors
            vector<pair<string, vector<double>>> rst;
            read_csv_bylines("./db.csv", rst);
            
            // calculate standard deviation for each feature
            double num_of_features = rst[0].second.size();
            vector<vector<double>> std_features(num_of_features, vector<double>(rst.size(), 0));
            for (int i = 0; i < rst.size(); i++) {
                vector<double> tmp_vec = rst[i].second;
                for (int j = 0; j < tmp_vec.size(); j++) {
                    std_features[j][i] = tmp_vec[j];
                }
            }
            vector<double> stds(num_of_features, 0);
            for (int i = 0; i < std_features.size(); i++) {
                stds[i] =  StandardDeviation(std_features[i]);
            }
            
            // loop rst and calculate the closest label
            map<double, string> the_map;
            for (int i = 0; i < rst.size(); i++) {
                vector<double> tmp_vec = rst[i].second;
                string label = rst[i].first;
                double distance = scaled_euclidian_distance(tmp_vec, the_obj_features, stds);
                the_map.insert({distance, label});
            }
            
            // loop through the map find the most relevent label by k
            map<string, int> k_map;
            int count = 0;
            for (auto itr = the_map.begin(); itr != the_map.end(); itr++) {
                string label = itr->second;
                if (k_map.contains(label)) {
                    k_map[label] += 1;
                } else {
                    k_map[label] = 0;
                }
                count++;
                if (count == k) {
                    break;
                }
            }
            
            std::map<string,int>::iterator best
            = std::max_element(k_map.begin(),k_map.end(),[] (const std::pair<string,int>& a, const std::pair<string,int>& b)->bool{ return a.second < b.second; } );
            
            cout << "The predicted label of your selected object is " + best-> first << endl;
            
            lastKey = 's';
        } else if (key == 'p') {
            char dirname[256];
            strcpy(dirname, "/Users/zhou/Library/Mobile\ Documents/com\~apple\~CloudDocs/Yizhou/学校/研究生/NU课程/CS5330\ CV/Projects/pics/");
            char image_path[256];
            
            struct dirent *entry;
            DIR *dp;
            
            dp = opendir(dirname);
            if (dp == NULL) {
                perror("opendir: Path does not exist or could not be read.");
                return;
            }
            
            while ((entry = readdir(dp))) {
                // check if the file is an image
                if (strstr(entry->d_name, ".jpg") ||
                    strstr(entry->d_name, ".png")) {
                    strcpy(image_path, dirname);
                    strcat(image_path, entry->d_name);
                    puts(image_path);
                    
                    src = imread(image_path);
                    
                    // preprocess the image
                    threshold_image(src, dst);
                    src = dst.clone();
                    custom_morphological_filtering(src, dst);
                    src = dst.clone();
                    
                    // manully select the wanted region
                    Mat stats;
                    int label_size = connected_components_analysis(dst, stats, 0);
                    imshow("Live Video", dst);
                    cout << "Press any key to continue" << endl;
                    waitKey(0);
                    cout << "Choose the region number you want to compute features for" << endl;
                    int number;
                    cin >> number;
                    cout << "please enter the label for your selected region ID" << endl;
                    string label;
                    cin >> label;
                    
                    Mat centroids;
                    Mat lables;
                    
                    int labelSize = connectedComponentsWithStats(dst, lables, stats, centroids);
                    
                    int x = stats.at<int>(Point(0, number));
                    int y = stats.at<int>(Point(1, number));
                    int w = stats.at<int>(Point(2, number));
                    int h = stats.at<int>(Point(3, number));
                    
                    write_csv_tasl_raw_pics("./pics.csv", entry->d_name, label, vector<int> {y, x, y-h, x+w});
                }
            }
            
            closedir(dp);
            
            lastKey = 'p';
        }
        
        imshow("Live Video", dst);
    }
}
