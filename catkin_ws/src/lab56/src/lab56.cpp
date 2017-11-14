
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <geometry_msgs/Point.h>

#include <rhino_ros/command.h>
#include <rhino_ros/positions.h>

// -----------------------------------------------------------------------------------------
// You will edit the two functions in this file (thresholdImage and associateObjects)
// for lab 5.  The functions at the bottom (onMouse function)of this file are not to be edited until lab 6.
// -----------------------------------------------------------------------------------------
int numobj;
double xw[5];
double yw[5];
static const std::string OPENCV_WINDOW = "Image window";
ros::Publisher *pub_command_ptr;
geometry_msgs::Point msg;
ros::Publisher lab_pub;
using namespace cv;
vector<Point2f> mc; // a vectoer of point that contains centroids

/*****************************************************
	*Class defined for both Lab5 and Lab6
 * **************************************************/
class ImageConverter
{
  //node handle
  ros::NodeHandle nh_;
  // initialize cv bridge sub and pub
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_; 
public: 
  //constructor(don't modify)
  ImageConverter();
  
  //destructor(don't modify)
  ~ImageConverter();
  
  //Subscriber callback function, will be called when there is a new image read by camera
  void imageCb(const sensor_msgs::ImageConstPtr& msg);
  
private:
	
	//lab5's function, thresholding the image, input a grayscale image and returns a threshholded image
	Mat thresholdImage(Mat gray_img);
	
	//lab5's function, associating image, input a black and white(binary) image and returns an associated image
	Mat associateObjects(Mat bw_img);
	
	//details of these functions is at following.
};
	


/*****************************************************
* Functions in class:
* **************************************************/	

//constructor(don't modify) 
ImageConverter::ImageConverter():it_(nh_)
{
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/cv_camera_node/image_raw", 1, 
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);
    namedWindow(OPENCV_WINDOW);    
}
//destructor(don't modify)
ImageConverter::~ImageConverter()
{
    cv::destroyWindow(OPENCV_WINDOW);
}
//subscriber callback function, will be called when there is a new image read by camera
void ImageConverter::imageCb(const sensor_msgs::ImageConstPtr& msg)
{  
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    } 
    // create an gray scale version of image
    Mat gray_image;
	cvtColor( cv_ptr->image, gray_image, CV_BGR2GRAY );  
    // convert to black and white img, then associate objects:  
    Mat bw_image = thresholdImage(gray_image); // bw image from own function
    Mat associate_image = associateObjects(bw_image); // find associated objects

    // Update GUI Window
    imshow("Image window", cv_ptr->image);
    imshow("gray_scale", gray_image);
    imshow("black and white", bw_image);
    imshow("associate objects", associate_image);
    waitKey(3);
    // Output some video stream
    image_pub_.publish(cv_ptr->toImageMsg());
} 

/*****************************************************
	 * Function for Lab 5
* **************************************************/	
// Take a grayscale image as input and return an thresholded image.
// You will implement your algorithm for calculating threshold here.
Mat ImageConverter::thresholdImage(Mat gray_img)
{
		int   totalpixels;
		Mat bw_img  = gray_img.clone(); // copy input image to a new image
		totalpixels	  = gray_img.rows*gray_img.cols;			// total number of pixels in image
		uchar graylevel; // use this variable to read the value of a pixel
		int zt=0; // threshold grayscale value 
		int i, index;
		double H[totalpixels];
		double u = 0.0;
		for(i=0; i<totalpixels; i++) {
			H[i] = 0;
		}
		for(i=0; i<totalpixels; i++) {
			index = (int) gray_img.data[i];
			H[index] += 1.0;
		}
		for(i=0; i<totalpixels; i++) {
			u += i * H[i];
		}	
		u = u/totalpixels;
		
		double q0cur = 0.0;
		double u0prev = 0.0;
		double u0cur = 0.0;
		double u1prev = 0.0;
		double u1cur = 0.0;
		double ob= 0.0;

		double q0prev = H[0]/totalpixels;
		double q1 = 0.0;

		for(i=1; i<totalpixels; i++) {
			q1 += H[i];
		}
		q1 /= totalpixels;
		
		for(i=1; i<totalpixels; i++) {
			u1prev += i*H[i]/(totalpixels);
		}
		u1prev /= q1;


		double obmax = q0prev*(1-q0prev)* pow(u0prev - u1prev,2);


		for(int z = 1; z < totalpixels; z++) {
			q0cur = H[z]/totalpixels + q0prev;
			if(q0cur == 1) {
				u1cur = 255;
				u0cur = (z) * H[z]/(totalpixels * q0cur) + q0prev * u0prev /(q0cur);
			}
			else if(q0cur == 0) {
				u0cur = 0;
				u1cur = (u- q0cur * u0cur)/(1 - q0cur);
			}
			else{
				u0cur = (z) * H[z]/(totalpixels * q0cur) + q0prev * u0prev /(q0cur);
				u1cur = (u - q0cur * u0cur)/(1 - q0cur);
			}
			ob = q0cur * (1- q0cur) * pow(u0cur - u1cur, 2);
			if(ob > obmax) {
				obmax = ob;
				zt = z;
			}
			q0prev = q0cur;
			u0prev = u0cur;			
		}
		
		//zt = 100;
		//std::cout<<zt<<std::endl;
		// threshold the image
		for(int i=0; i<totalpixels; i++)
		{
			graylevel = gray_img.data[i];	
			if(graylevel>zt) bw_img.data[i]= 255; // set rgb to 255 (white)
			else             bw_img.data[i]= 0; // set rgb to 0   (black)
		}	
	return bw_img;	
}
/*****************************************************
	 * Function for Lab 5
* **************************************************/
// Take an black and white image and find the object it it, returns an associated image with different color for each image
// You will implement your algorithm for rastering here
Mat ImageConverter::associateObjects(Mat bw_img)
{
	//initiallize the variables you will use
	int height,width; // number of rows and colums of image
	int red, green, blue; //used to assign color of each objects
	uchar pixel; //used to read pixel value of input image 
	height = bw_img.rows;
	width = bw_img.cols;
		
	int label[20000];
	int *equiv[20000];
	int Pixel, Left, Above, labelnum, smallerbaselabel, min, max;
	
	labelnum = 1;

	for (int i=0; i <20000; i++)
	{
		equiv[i] = &label[i];
	}
	
	int hist2[height*width];

	// initialize an array of labels, assigning a label number to each pixel in the image
	int ** pixellabel = new int*[height];
	for (int i=0;i<height;i++) {
		pixellabel[i] = new int[width];
	}
	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			// read values of pixels in input black and white image
			int ind=row*width+col;
			pixel = bw_img.data[ind]; //{255 or 0}
			if (pixel ==0x00)		pixellabel[row][col]=0;		// object/foreground
			else				pixellabel[row][col]=-1;	// background
		}
	}

		
	//first raster
	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			Pixel = pixellabel[row][col];
			
			if(col > 0)
			{
				Left  = pixellabel[row][col-1];
			} 
			else {
				Left = -1;
			}
			if(row > 0)
			{
				Above = pixellabel[row-1][col];
			} 
			else {
				Above = -1;
			}
			//std::cout<<"check "<<row<<" "<<col<<std::endl;
			if (Pixel != -1)
			{
   			if((Left==-1) and (Above==-1))
				{
       		pixellabel[row][col] = labelnum;
   				label[labelnum] = labelnum;
   				labelnum++;
   			}
    			
    			if ((Left!=-1) and (Above==-1))
				{
        			pixellabel[row][col] = Left;
    			}
    			
    			if ((Left==-1) and (Above!=-1))
				{
        			pixellabel[row][col] = Above;
    			}
    			// EQUIVALENCE CASE:
    			if ((Left!=-1) and (Above!=-1))
    			{

        			smallerbaselabel = std::min(*equiv[Left], *equiv[Above]);

        			if (smallerbaselabel==*equiv[Left])
					{
             			min = Left;
						max = Above;
        			}
        					
        			else
					{	
            			min = Above;
						max = Left;
        			}
        					
        			pixellabel[row][col] = smallerbaselabel;
        			*equiv[max] = *equiv[min];
        			equiv[max] = equiv[min];
   				}

			}

		}	
	}

	//second raster 
	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			Pixel = pixellabel[row][col];
			if(Pixel != -1)
			{
    			pixellabel[row][col] = *equiv[Pixel];
			}
		}
	}

	int objectnum[labelnum];
	//initialize array
	for(int i=0; i<labelnum-1; i++)
	{
		objectnum[i]=0;
	}
		
	//histogram of number of pixels per object label
	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			Pixel = pixellabel[row][col];
			if( Pixel != -1)
			{
				objectnum[Pixel]++;
			}
		}
	}
		
	int count = 1;
	int ot = 500;
	int ot2 = 2500;
		
	//flag objects for removal when less than ot. Count objects greater than ot
	for(int i=0; i<labelnum; i++)
	{
		if((objectnum[i] < ot) || (objectnum[i] > ot2))
		{
			objectnum[i] = 0; 
		}
		else
		{
			count++;
		}
	}

	numobj = count;

	//std::cout<<count<<std::endl;
		
	//go through image and change pixels to white if object does not have high enough number of pixels
	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			Pixel = pixellabel[row][col];
			if(objectnum[Pixel] == 0)
			{
				pixellabel[row][col]=-1;
			}
		}
	}
		
    int obj[count];
    for(int i =0; i<count; i++)
    {
        obj[i]=0;
    }
    
    int temp;
    int idx=1;
    bool flag;
    for(int row=0; row<height; row++)
    {
        for(int col=0; col<width; col++)
        {
            Pixel=pixellabel[row][col];
            if(Pixel != -1)
            {
                for(int i=0;i<count;i++)
                {
                    if(Pixel==obj[i])
                    {
                        temp=i;
                        flag = true;
                    }
                }
                if(flag!=true){
                    if(idx<count)
                    {
                        obj[idx]=Pixel;
                        temp=idx;
                        idx++;
                        
                    }
                    else
                    {
                        temp=-1;
                    }
                }
                pixellabel[row][col]=temp;
                flag=false;
            }
        }
    }
	
	// determine number of objects in the image
	// specify a unique color for each object
	// assign color to all pixels in each objet
		
	// assign UNIQUE color to each object
	Mat associate_img = Mat::zeros( bw_img.size(), CV_8UC3 ); // function will return this image
	Vec3b color;
	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			switch (  pixellabel[row][col] )
			{
					
				case 0:
					red    = 255; // you can change color of each objects here
					green = 255;
					blue   = 255;
					break;
				case 1:
					red    = 255; // you can change color of each objects here
					green  = 0;
					blue   = 0;
					break;
				case 2:
					red    = 0;
					green  = 255;
					blue   = 0;
					break;
				case 3:
					red    = 0;
					green  = 0;
					blue   = 255;
					break;
				case 4:
					red    = 255;
					green  = 255;
					blue   = 0;
					break;
				case 5:
					red    = 255;
					green  = 0;
					blue   = 255;
					break;
				case 6:
					red    = 0;
					green  = 255;
					blue   = 255;
					break;
          		case 7:
          			red    = 128;
            		green  = 128;
           			blue   = 0;
            		break;
          		case 8:
          		 	red    = 128;
          			green  = 0;
            		blue   = 128;
            		break;
          		case 9:
            		red    = 0;
            		green  = 128;
            		blue   = 128;
            		break;
				default:
					red    = 0;
					green = 0;
					blue   = 0;
					break;					
			}

			color[0] = blue;
			color[1] = green;
			color[2] = red;
			associate_img.at<Vec3b>(Point(col,row)) = color;
		}
	}
		
	double objrow[count];
	double objcol[count];
	double objimg[count];
	double objimgrow[count];
	double objimgcol[count];

	for(int i=0; i<count; i++)
	{
		objrow[i]=0.0;
		objcol[i]=0.0;
		objimg[i]=0.0;
		objimgrow[i]=0.0;
		objimgcol[i]=0.0;
		xw[i]=0.0;
       	yw[i]=0.0;
	}

    for(int row=0; row<height; row++)
    {
      	for(int col=0; col<width; col++)
      	{
       		for(int i=0;i<count;i++)
        	{	
    	   		Pixel=pixellabel[row][col];
        		if(Pixel == i)
        		{
  	   				//calculate row centroid
    				objimg[i]++;
 					objimgrow[i] += row;
   					objrow[i] = objimgrow[i]/objimg[i];

   					//calculate col centroid
   					objimgcol[i] += col;
   					objcol[i] = objimgcol[i]/objimg[i];
      		 	}
      		}
    	}
  	}
	double x0;
	double y0;
	float beta;
	int x1=135;
	int x2=146;
	int y1=229;
	int y2=376;
	float d=20.0;
	beta=sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)))/d;

	//calc theta
	x1=174;
	x2=369;
	y1=192;
	y2=201;
	float theta;
	theta=atan2((y1-y2),(x2-x1));

	int Oc=320;
	int Or=240;

	int rt=84;
	int ct=185;
	float xt=41;
	float yt=11.5;
	float yc;
	float xc;
	xc=-(rt-Or)/beta;
	yc=-(ct-Oc)/beta;

	//Calc Tx and Ty
	float Tx;
	float Ty;

	Tx=xc-(xt*cos(theta))+(yt*sin(theta));
	Ty=yc-(xt*sin(theta))-(yt*cos(theta));
	//printf("Tx: %f\n",Tx);
	//printf("Ty: %f\n",Ty);



	// you can use cv::line function to custruct cross hair.
  	for (int i = 1; i < count; i++)
  	{
  		int r=objrow[i];
		int c=objcol[i];

		yc=-(c-Oc)/beta;
		xc=-(r-Or)/beta;

		cv::line(associate_img, Point(objcol[i]-5, objrow[i]), Point(objcol[i]+5, objrow[i]), Scalar( 255, 255, 255 ),  2, 8);
		cv::line(associate_img, Point(objcol[i], objrow[i]-5), Point(objcol[i], objrow[i]+5), Scalar( 255, 255, 255 ),  2, 8);
  		xw[i] = cos(theta)*xc+sin(theta)*yc-cos(theta)*Tx-sin(theta)*Ty;
		yw[i] = (Tx-xc)*sin(theta)+(yc-Ty)*cos(theta);
  	}
		

	// clean up memory
	for (int i=0;i<height;i++) 
	{
		delete pixellabel[i];
	}
	delete pixellabel;
	return associate_img;
}

/*****************************************************
	*Function for Lab 6
 * **************************************************/
 //This is a call back function of mouse click, it will be called when there's a click on the video window.
 //You will write your coordinate transformation in this function.
void onMouse(int event, int x, int y, int flags, void* userdata)
{

	//calc beta
	double x0;
	double y0;
	float beta;
	int x1=135;
	int x2=146;
	int y1=229;
	int y2=376;
	float d=20.0;
	beta=sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)))/d;
	//printf("Beta: %f",beta);

	//calc theta
	x1=174;
	x2=369;
	y1=192;
	y2=201;
	float theta;
	theta=atan2((y1-y2),(x2-x1));
	//theta=0;
	//printf("theta: %f\n",theta);

	int Oc=320;
	int Or=240;


	int rt=84;
	int ct=185;
	float xt=41;
	float yt=11.5;
	float yc;
	float xc;
	xc=-(rt-Or)/beta;
	yc=-(ct-Oc)/beta;

	//Calc Tx and Ty
	float Tx;
	float Ty;

	Tx=xc-(xt*cos(theta))+(yt*sin(theta));
	Ty=yc-(xt*sin(theta))-(yt*cos(theta));

	int r=y;
	int c=x;

	yc=-(c-Oc)/beta;
	xc=-(r-Or)/beta;

	//initialization & variable definition

	ros::Rate loop_rate(4);

	// For use with Lab 6
	// If the robot is holding a block, place it at the designated row and column. 
	if  ( event == EVENT_LBUTTONDOWN ) //if left click, do nothing other than printing the clicked point
	{  
		ROS_INFO_STREAM("left click:  (" << x << ", " << y << ")");  //the point you clicked
		 x0 = cos(theta)*xc+sin(theta)*yc-cos(theta)*Tx-sin(theta)*Ty;
		y0 =(Tx-xc)*sin(theta)+(yc-Ty)*cos(theta);
		msg.x=x0;
		msg.y=y0;
		msg.z=0;
		//publish the result.
			
		lab_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
	else if  ( event == EVENT_RBUTTONDOWN )//if right click, find nearest centroid,
	{
		ROS_INFO_STREAM("right click:  (" << x << ", " << y << ")");
		// camera callibration
		x0 = cos(theta)*xc+sin(theta)*yc-cos(theta)*Tx-sin(theta)*Ty;
		y0 =(Tx-xc)*sin(theta)+(yc-Ty)*cos(theta);
    	double d_min=1000;
    	int stdstd=0;
    	double d=0.0;

	  	for(int i = 1; i < numobj; i++)
   		{
    		d=sqrt(pow(x0-xw[i],2)+pow(y0-yw[i],2));
    		if(d<d_min)
     		{
        		d_min=d;
        		stdstd=i;
      		}
    	}

    	x0=xw[stdstd];
   		y0=yw[stdstd];
		ROS_INFO_STREAM("the point in world frame:  (" << x0 << ", " <<y0 << ")");
		//publish the point to the robot
		msg.x=x0;
		msg.y=y0;
		msg.z=1;
		//publish the result.
			
		lab_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();	
	}
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ros::NodeHandle nh_command;
  lab_pub = nh_command.advertise<geometry_msgs::Point>("/rhino/target_position", 1000);

  //initialize video window names
  namedWindow("Image window");
  namedWindow("associate objects");
  namedWindow("gray_scale");
  namedWindow("black and white");
  
  //declare the class
  ImageConverter ic;
  
  //initialize the mouse callback
  setMouseCallback("Image window", onMouse);
  setMouseCallback("associate objects", onMouse);
  setMouseCallback("black and white", onMouse);
  setMouseCallback("gray_scale", onMouse);
  
  while(ros::ok()){
  	ros::spinOnce();
  }
  
  return 0;
}
