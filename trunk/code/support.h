#include <vector>
#ifdef __cplusplus
extern "C" {
#endif
void setAutoIDefaults(Symbol *S, const int& val);
void setAutoFDefaults(Symbol *S, const float& val);
void setAutoSDefaults(Symbol *S, const string& val, const int fullVal=0);
void setAutoNIDefaults(Symbol *S, const vector<int>& val);
void setAutoNFDefaults(Symbol *S, const vector<float>& val);
void setAutoNSDefaults(Symbol *S, const string& val);
#ifdef __cplusplus
	   }
#endif
