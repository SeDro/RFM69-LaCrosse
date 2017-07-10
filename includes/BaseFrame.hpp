#ifndef BaseFrame_h
#define BaseFrame_h
class BaseFrame {
	public:
	    unsigned char  Header;
		unsigned char  ID;
		virtual void updateFrame(BaseFrame* newFrame) {
			this->Header = newFrame->Header;
			this->ID = newFrame->ID;
		}
};
#endif
