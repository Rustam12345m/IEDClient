#pragma once

#include "logical_node.hpp"
#include <string>

class TGGIO : public TLogicalNode
{
public:
	TGGIO(LogicalDevice *t_ld, const std::string &t_name,
			int t_an = 0, int t_ind = 0) : TLogicalNode(t_ld)
	{
		m_libLN = LogicalNode_create(t_name.c_str(), m_libLD);

		m_libMod = CDC_ENS_create("Mod", (ModelNode *)m_libLN, 0);
		m_libBeh = CDC_ENS_create("Beh", (ModelNode *)m_libLN, 0);
		m_libHealth = CDC_ENS_create("Health", (ModelNode *)m_libLN, 0);

		for (int i=0;i<t_an;i++) {
			CDC_MV_create(std::string("AnIn" + std::to_string(i + 1)).c_str(), (ModelNode *)m_libLN, 0, 0);
		}
		for (int i=0;i<t_ind;i++) {
			CDC_SPS_create(std::string("Ind" + std::to_string(i + 1)).c_str(), (ModelNode *)m_libLN, 0);
		}
	};

	~TGGIO() override 
	{
	}
};