#pragma once

extern "C"
{
	#include "iec61850_server.h"
}

class TLogicalNode
{
public:
	TLogicalNode() = delete;
	TLogicalNode(LogicalDevice *t_ld) : m_libLD(t_ld) {}
	virtual ~TLogicalNode() {}

	LogicalDevice* 	ldevice() const { return m_libLD; }
	LogicalNode* 	lnode() const { return m_libLN; }

protected:
	LogicalDevice* 	m_libLD = nullptr;
	LogicalNode* 	m_libLN = nullptr;

	DataObject* 	m_libMod = nullptr;
	DataObject*		m_libBeh = nullptr;
	DataObject*		m_libHealth = nullptr;
};