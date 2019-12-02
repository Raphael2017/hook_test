#pragma once
#include <iostream>
//#include "QueryCloudAZExport.h"
#include <vector>
#include <string>

wchar_t* char_to_wchar(const char* c)
{
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	wchar_t* pwchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), pwchar, len);
	pwchar[len] = '\0';
	return pwchar;
}
std::wstring char_to_wstring(const char* c)
{
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	wchar_t* pwchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), pwchar, len);
	pwchar[len] = '\0';
	std::wstring ret(pwchar);
	delete[] pwchar;
	return ret;
}

struct MaskConditions
{
	std::wstring _field;
	std::wstring _op;
	std::wstring _value;
};


struct MaskFields {
	std::wstring _c2character;
	std::vector<std::wstring> _fields;
	std::list<MaskConditions>  _condi;
};

bool QueryAZ(const char* username, const std::map<std::string,std::string>& resAttr,
	const std::list<std::string>& lstColumns,
	MaskFields & out) {
#if 1
	IPolicyRequest* pRequest = CreatePolicyRequest();

	//action necessary
	pRequest->SetAction("VIEW");

	//user attr  necessary
	IAttributes* pUser = CreateCEAttr();
	pUser->AddAttribute("username", username, XACML_string);
	pRequest->SetUserInfo(username, "User", pUser);

	// Set source info, this is mandatory
	IAttributes* pSourceAttr = CreateCEAttr();
	auto itResAttr = resAttr.begin();
	while (itResAttr!=resAttr.end())
	{
		pSourceAttr->AddAttribute(itResAttr->first.c_str(), itResAttr->second.c_str(), XACML_string);
		itResAttr++;
	}

	auto itCol = lstColumns.begin();
	while (itCol!=lstColumns.end())
	{
		pSourceAttr->AddAttribute("columns", itCol->c_str(), XACML_string);
		itCol++;
	}
	
	pRequest->SetSource("hana_poc", "hana_poc", pSourceAttr);

	// 	IAttributes* pTarget = CreateCEAttr();
	// 	pTarget->AddAttribute("", "hana_mask", XACML_string);
	// 	pRequest->SetTarget("", "", pTarget);

	//app info 
	//	pRequest->SetAppInfo("SQLProxy", "apppath_ee", "app url", NULL);

	//host info necessary
	pRequest->SetHostInfo("HostName", "10.23.60.102", NULL);

	const IPolicyRequest* request[] = { pRequest };
	IPolicyResult* result[sizeof(request) / sizeof(request[0])] = { 0 };
	while (true)
	{
		IPolicyResult* pResult = NULL;
		QueryStatus qs0 = CheckSingleResource(pRequest, &pResult);

		if (QS_S_OK != qs0 || pResult == NULL) {
			return false;
		}
		//QueryStatus qs1 = CheckMultiResource(&request[0], sizeof(request)/sizeof(request[0]), result);
		if (pResult)
		{
			int icount = pResult->ObligationCount();
			for (int i = 0; i < icount; ++i)
			{
				MaskConditions mc;
				const IObligation* pobg = pResult->GetObligationByIndex(i);
				const IAttributes* pattr = pobg->GetAttributes();
				size_t icout = pattr->Count();
				for (size_t j = 0; j < icout; ++j)
				{
					const char * sname = nullptr;
					const char * svalue = nullptr;

					CEAttributeType tp = XACML_string;
					pattr->GetAttrByIndex(j, &sname, &svalue, &tp);

					std::wstring strname = char_to_wstring(sname);
					std::wstring strvalue = char_to_wstring(svalue);

					if (strvalue.empty()) continue;

					if (strname.compare(L"mask_characte") == 0) {
						out._c2character = strvalue;
					}
					else if (strname.compare(L"policy_model_id") == 0) {

					}
					else if(strname.compare(L"mask_field1")==0 || 
						    strname.compare(L"mask_field2")== 0 ||
						    strname.compare(L"mask_field3") == 0 ||
						    strname.compare(L"mask_field4") == 0){

						out._fields.push_back(strvalue);
					}
					else if(strname.compare(L"condition1")==0 || strname.compare(L"condition2") == 0)
					{
						mc._field = strvalue;
					}
					else if (strname.compare(L"operation1") == 0 || strname.compare(L"operation2") == 0)
					{
						mc._op = strvalue;
					}
					else if (strname.compare(L"value1") == 0 || strname.compare(L"value2") == 0)
					{
						mc._value = strvalue;

						out._condi.push_back(mc);
					}

					printf("obg:(%s, %s)\n", sname, svalue);
				}
				const char* pname = pobg->GetObligationName();

			}
			FreePolicyResult(pResult);
		}
		break;
	}
#endif 
	return true;
}