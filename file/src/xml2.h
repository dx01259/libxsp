/*
 * xml2.h
 *
 *  Created on: 2015年6月15日
 *      Author: dengxu
 */

#ifndef FILE_XML2_H_
#define FILE_XML2_H_

#include<stdio.h>
#include<libxml/tree.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<linux/limits.h>
#include<libxml/xmlstring.h>
#include<libxml/parser.h>

#include<string>
using namespace std;

#define MERGE_PATH_NAME(path, name) (path)+(name)

struct xsp_xml
{
	xmlDocPtr  xml_filep;    //the object pointer of xml
	string     xml_path;     //the path of xml file
	string     xml_name;     //the name of xml file
};

/**
 * xml_init -- initialize the member of xsp_xml structure
 * @filep: pointer to structure of xsp_xml
 * @path: the path of xml file that must be end with of '\' or '/'
 * @name: the name of xml file
 */
int xml_init(struct xsp_xml *filep, const string &path, const string &name)
{
	if(filep)
	{
		filep->xml_filep = NULL;
		filep->xml_path  = path;
		filep->xml_name  = name;
		return 0;
	}
	return -1;
}

/**
 * is_xmlfile -- to judge the file's suffix is .xml or .XML
 * @name: the name of xml file
 */
bool is_xmlfile(const char *name)
{
	size_t len = strlen(name);
	if(
			len <= 4                          ||
			(
			strcmp(&name[len-4], ".xml") != 0 &&
			strcmp(&name[len-4], ".XML") != 0 )
	)
	{
		return false;
	}
	return true;
}

/**
 * xml_create -- create a xml format's file
 * @filep: pointer to structure xsp_xml
 * @root: the root node of xml file
 * @rName: the name of root node
 */
int xml_create(struct xsp_xml *filep, xmlNodePtr root, const char rName)
{
	if(filep)
	{
		if(filep->xml_filep &&
				is_xmlfile(filep->xml_name.c_str()))
		{
			xmlDocPtr &doc = filep->xml_filep;
			doc = xmlNewDoc(BAD_CAST("1.0"));
			if(doc)
			{
				root = xmlNewNode(NULL, BAD_CAST(rName));
				if(!root) return -3;
				xmlDocSetRootElement(doc, root);
				string pathName = MERGE_PATH_NAME(filep->xml_path,filep->xml_name);
				xmlSaveFormatFile(pathName.c_str(), doc, 1);
			}
			return 0;
		}
		return -2;
	}
	return -1;
}

/**
 * xml_open -- To open a xml file
 * @filep: pointer to structure of xsp_xml
 * 		that xml file
 */
int xml_open(struct xsp_xml *filep)
{
	if(filep && filep->xml_name.size()>=4)
	{
		string szPathName = MERGE_PATH_NAME(filep->xml_path, filep->xml_name);
		if(is_xmlfile(szPathName.c_str()))
		{
			filep->xml_filep = xmlParseFile(szPathName.c_str());
			return filep->xml_filep ? 0:-3;
		}
		return -2;
	}
	return -1;
}

/**
 * xml_write -- Write a new node to xml file
 * @filep: poniter to a structure xsp_xml
 * @nName: the name new node
 * @data: the data of new node
 */
size_t xml_write(const struct xsp_xml *filep, const char *nName, const char *data)
{
	if(filep && filep->xml_filep)
	{
		xmlDocPtr doc = filep->xml_filep;
		xmlNodePtr cur = xmlDocGetRootElement(doc);
		cur = cur->xmlChildrenNode;
		xmlNodePtr parent = NULL;
		while(cur)
		{
			if(!xmlStrcmp(cur->name, (const xmlChar *)nName))
				break;
			parent = cur;
			cur = cur->next;
		}
		if(NULL != cur)
		{
			xmlNodeSetContent(cur, (xmlChar *)data);
			return strlen(data);
		}
		else{
			xmlNewTextChild(parent, NULL, (const xmlChar *)nName, (const xmlChar *)data);
			return strlen(data);
		}
	}

	return 0;
}

/**
 * xml_read -- read a node's values
 * @filep: pointer to a structure xsp_xml
 * @nName: the name of node to read
 * @szData: the data readed from node by @nName
 */
size_t xml_read(const struct xsp_xml *filep, const char *nName, string &szData)
{
	if(filep && filep->xml_filep)
	{
		xmlDocPtr doc = filep->xml_filep;
		xmlNodePtr cur = xmlDocGetRootElement(doc);
		if(NULL != cur)
		{
			cur = cur->xmlChildrenNode;
			while(cur)
			{
				if(!xmlStrcmp(cur->name, (const xmlChar *)nName))
					break;
				cur = cur->next;
			}
			if(NULL != cur)
			{
				size_t len = strlen((char *)xmlNodeGetContent(cur));
				szData.resize(len);
				memcpy(&szData[0], (char *)xmlNodeGetContent(cur), len);
				return len;
			}
		}
	}
	return 0;
}

/**
 * fget_node -- Get the address of node
 * @filep: pointer to structure xsp_xml
 * @nName: the name of node that to get
 * 			it's address
 */
xmlNodePtr fget_node(const struct xsp_xml *filep, const char *nName)
{
	if(filep && filep->xml_filep)
	{
		xmlDocPtr doc = filep->xml_filep;
		xmlNodePtr cur = xmlDocGetRootElement(doc);
		if(NULL != cur)
		{
			cur = cur->xmlChildrenNode;
			while(cur)
			{
				if(!xmlStrcmp(cur->name, (const xmlChar *)nName))
					break;
				cur = cur->next;
			}
			return cur;
		}
	}
	return NULL;
}

/**
 * xml_close -- close a object of xsp_xml structure
 * @filep: pointer the object to close
 */
bool xml_close(const struct xsp_xml *filep)
{
	if(filep && filep->xml_filep)
	{
		xmlFreeDoc(filep->xml_filep);
		return true;
	}
	return false;
}

#endif /* FILE_XML2_H_ */
