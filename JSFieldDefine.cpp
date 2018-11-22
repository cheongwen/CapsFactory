//
// Created by 周飞宇 on 2018/11/21.
//

#include "JSFieldDefine.h"
#include "Common.h"

JSFieldDefine::~JSFieldDefine() {

}

const string JSFieldDefine::create_get_function(const string &tab) {
    static const char* const Template_Getter = "get%s () {\n"
                                               TB"return this.%s\n"
                                               "}\n";
    RETURN_CODEFORMAT(tab.c_str(), Template_Getter, head_up_name.c_str(), camel_name.c_str());
}

const string JSFieldDefine::create_set_function(const string &tab) {

    static const char* const Template_Getter = "set%s (v) {\n"
                                               TB"this.%s = v\n"
                                               "}\n";
    RETURN_CODEFORMAT(tab.c_str(), Template_Getter, head_up_name.c_str(), camel_name.c_str());
}

const string JSFieldDefine::create_serialize_function(const string &tab) {
    if (repeated) {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_Array_User_Define =
                    "caps.writeInt32(%s.length)\n"
                    "for(var i = 0; i < %s.length; ++i)\n"
                    TB"caps.writeCaps(%s[i].serializeForCapsObj())\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define,
                              camel_name.c_str(), camel_name.c_str(), camel_name.c_str());
        } else if (field_type == FieldType::STRING) {
            static const char* const Template_Serialize_Array_String =
                    "caps.writeInt32(%s.length)\n"
                    "for(var i = 0; i < %s.length; ++i)\n"
                    TB"caps.write(%s[i])\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_String,
                              camel_name.c_str(), camel_name.c_str(), camel_name.c_str());
        } else {
            static const char* const Template_Serialize_Array_Inner =
                    "caps.writeInt32(%s.length)\n"
                    "for(var i = 0; i < %s.length; ++i)\n"
                    TBTB"caps.%s(%s[i])\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_Inner,
                              camel_name.c_str(), camel_name.c_str(),
                    FieldJSWriterFunction[static_cast<int>(field_type)], camel_name.c_str());
        }
    } else {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_User_Define =
                    "caps.writeCaps(%s.serializeForCapsObj())\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_User_Define, camel_name.c_str()
                              );
        } else {
            static const char* const Template_Serialize_Inner = "caps.%s(%s)\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner,
                    FieldJSWriterFunction[static_cast<int>(field_type)], camel_name.c_str());
        }
    }
}

const string JSFieldDefine::create_deserialize_function(const string &tab) {
    if (repeated) {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_Array_User_Define =
                    "count%s = caps.readInt32()\n"
                    "for(var i = 0; i < count%s; ++i) {\n"
                    TB"var tmpObj = new %s()\n"
                    TB"tmpObj.deserializeForCapsObj(caps.readCaps())\n"
                    TB"%s.push(tmpObj)\n"
                    "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define,
                              head_up_name.c_str(), head_up_name.c_str(),
                              user_define_type_name.c_str(), camel_name.c_str());
        }
        else {
            static const char *const Template_Serialize_Array_String =
                    "count%s = caps.readInt32()\n"
                    "for(var i = 0; i < count%s; ++i)\n"
                    TB"%s.push(caps.%s())\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_String,
                              head_up_name.c_str(), head_up_name.c_str(), camel_name.c_str(),
                              FieldJSReadFunction[static_cast<int>(field_type)]);
        }
    } else {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_User_Define =
                    "%s.deserializeForCapsObj(caps.readCaps())\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_User_Define,
                              camel_name.c_str());
        }
        else{
            static const char* const Template_Serialize_Inner = "%s = caps.%s()\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner, camel_name.c_str(), FieldJSReadFunction[static_cast<int>(field_type)]);
        }
    }
}

const string JSFieldDefine::create_class_member(const string &tab) {
    if (repeated)
    {
        static const char* const Template_Member_Array_User_Define = "this.%s = []\n";
        RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_User_Define, camel_name.c_str());
    }
    else
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char* const Template_Member_User_Define = "this.%s = new %s()\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define,
                    camel_name.c_str(), user_define_type_name.c_str());
        }
        else
        {
            static const char* const Template_Member_User_Define = "this.%s = %s\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define,
                    camel_name.c_str(),FieldJSTypeDefault[static_cast<int>(field_type)]);
        }

    }
}

