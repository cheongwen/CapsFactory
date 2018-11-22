//
// Created by 周飞宇 on 2018/11/20.
//

#include "BaseAll.h"
#include <iostream>
#include "Common.h"

bool BaseFieldDefine::parse(cJSON *root) {
    if (root->type != cJSON_Object)
        return false;
    cJSON * name = cJSON_GetObjectItem(root, "Name");
    if (name && name->type == cJSON_String)
    {
        this->name = name->valuestring;
        camel_name = Common::camel_case(this->name.c_str());
        head_up_name = Common::head_upcase(this->name.c_str());
    }
    else
        return false;

    cJSON * ftype = cJSON_GetObjectItem(root, "Type");
    if (ftype && ftype->type == cJSON_String)
    {
        string field_type_name = ftype->valuestring;
        std::transform(field_type_name.begin(), field_type_name.end(), field_type_name.begin(), ::tolower);
        if (field_type_name == "string")
            field_type = FieldType::STRING;
        else if (field_type_name == "int32")
            field_type = FieldType::INT32;
        else if (field_type_name == "uint32")
            field_type = FieldType::UINT32;
        else if (field_type_name == "uint64")
            field_type = FieldType::UINT64;
        else if (field_type_name == "int64")
            field_type = FieldType::INT64;
        else if (field_type_name == "float")
            field_type = FieldType::FLOAT;
        else if (field_type_name == "double")
            field_type = FieldType::DOUBLE;
        else
        {
            user_define_type_name = ftype->valuestring;
            field_type = FieldType::USERDEFINE;
        }
    }
    else
        return false;



    cJSON * optional = cJSON_GetObjectItem(root, "Optional");
    if (!optional)
    {
        cout<<"[Optional] of field ["<<this->name<<"] is invalid(defalue 'false')"<<endl;
        this->optional = false;
    }
    else if (optional->type == cJSON_False)
        this->optional = false;
    else if (optional->type == cJSON_True)
        this->optional = true;

    cJSON * repeated = cJSON_GetObjectItem(root, "Repeated");
    if (!repeated)
    {
        cout<<"[repeated] of field ["<<this->name<<"] is invalid(defalue 'false')"<<endl;
        this->repeated = false;
    }
    else if (repeated->type == cJSON_False)
        this->repeated = false;
    else if (repeated->type == cJSON_True)
        this->repeated = true;

    return true;
}

void BaseFieldDefine::set_name(const string &name) {
    BaseFieldDefine::name = name;
}

void BaseFieldDefine::set_field_type(FieldType field_type) {
    BaseFieldDefine::field_type = field_type;
}

void BaseFieldDefine::set_optional(bool optional) {
    BaseFieldDefine::optional = optional;
}

void BaseFieldDefine::set_repeated(bool repeated) {
    BaseFieldDefine::repeated = repeated;
}

const string &BaseFieldDefine::get_name() const {
    return name;
}

FieldType BaseFieldDefine::get_field_type() const {
    return field_type;
}

bool BaseFieldDefine::is_optional() const {
    return optional;
}

bool BaseFieldDefine::is_repeated() const {
    return repeated;
}




bool BaseMsgDefine::parse(cJSON *root) {
    if (root->type != cJSON_Object)
        return false;
    cJSON * name = cJSON_GetObjectItem(root, "MsgName");
    if (name && name->type == cJSON_String)
    {
        this->msg_name = name->valuestring;
        this->msg_name_upper = Common::to_upper(name->valuestring);
    }
    else
        return false;

    cJSON * members = cJSON_GetObjectItem(root, "Fields");
    if (!members)
        return false;
    else if (members->type == cJSON_Array)
    {
        for(int i = 0; i < cJSON_GetArraySize(members); ++i)
        {
            cJSON *field = cJSON_GetArrayItem(members, i);
            fields.push_back(new_field_define());
            if (!fields.back()->parse(field))
                return false;
        }
    }
    return true;
}

const string &BaseMsgDefine::get_msg_name() const {
    return msg_name;
}

const vector<shared_ptr<BaseFieldDefine>> &BaseMsgDefine::get_fields() const {
    return fields;
}

const string &BaseMsgDefine::get_msg_name_upper() const {
    return msg_name_upper;
}


bool BaseMsgGroup::parse(cJSON *root) {
    if (root->type != cJSON_Object)
        return false;
    cJSON * ns = cJSON_GetObjectItem(root, "NameSpace");
    if (ns && ns->type == cJSON_String)
        this->ns = ns->valuestring;
    else
        this->ns = "";

    cJSON * msgs = cJSON_GetObjectItem(root, "Msg");
    if (msgs && msgs->type == cJSON_Array)
    {
        for(int i = 0; i < cJSON_GetArraySize(msgs); ++i)
        {
            cJSON *msg = cJSON_GetArrayItem(msgs, i);
            msg_define.push_back(new_msg_define());
            if (!msg_define.back()->parse(msg))
                return false;
        }
    } else
        return false;
    return true;
}