
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 ) ;
CvSeq* findSquares4( IplImage* img, CvMemStorage* storage ,double thresh,double thresh2,const char *wndname) ;
void drawSquares( IplImage* img, CvSeq* squares );
void Squares(IplImage* img,double thresh1,double thresh2,const char *wndname);
