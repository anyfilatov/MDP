#include "stdafx.h"

class StringWithHash:public std::string,public Hashable{
	int hash(){
		int hash = 0;
		int n = this->length();
		for (int i =0; i<n; i++){
			char j = ((string)*this)[i];
			hash+=j*pow((float)31,(float)n-1-i);
		}
		return hash;
	}	
}
