#include "Helper.hpp"
void Helper::stringifyFrame(Frame* frame, char* result) {
	//char result[1000];
//	cout << "Writing to buffer" << endl;
//	sprintf(result, "%d", frame ->ID);
	sprintf(result, "{\n\r\t\"ID\" : \"%d\",\n\r\t\"TYPE\" : \"Humidity_Temp\",\n\r\t\"DIRECTION\" : \"Input\",\n\r\t\"New_Battery\" : \"%s\",\n\r\t\"Bit12\" : \"%s\",\n\r\t\"Temp\" : \"%.1f\",\n\r\t\"Weak_Battery\" : \"%s\",\n\r\t\"Humidity\" : \"%02i\%\",\n\r\t\"Humidity_Abs\" : \"%10f\"\n\r}",
	frame->ID,
	frame->NewBatteryFlag == true ? "TRUE" : "FALSE",
	frame->Bit12 == true ? "TRUE" : "FALSE",
	frame->Temperature,
	frame->WeakBatteryFlag == true ? "TRUE" : "FALSE",
	frame->Humidity,
	frame->HumidityAbs);
//	cout << "written : " << result;
}

bool Helper::isNumber(string str) {
    char* p;
    strtol(str.c_str(), &p, 10);
    return *p == 0;
}
