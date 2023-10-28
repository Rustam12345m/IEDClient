/*
 *  main.cpp
 *
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDClient.
 *
 *  IEDClient is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDClient is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDClient.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#include "data_model_builder.hpp"

namespace Core
{
	DataModelBuilder::DataModelBuilder()
	{
		m_model = QSharedPointer<DataModel>::create("");
	}

	DataModelBuilder::~DataModelBuilder()
	{
	}

	DataModelBuilder& DataModelBuilder::createLD(const QString &t_name)
	{
		m_lastLD = QSharedPointer<LogicalDevice>::create(m_model.get(), t_name);
		m_model->addChild(m_lastLD);
		return *this;
	}

	DataModelBuilder& DataModelBuilder::createLN(const QString &t_name)
	{
		m_lastLN = QSharedPointer<LogicalNode>::create(m_lastLD.get(), t_name);
		m_lastLD->addChild(m_lastLN);
		return *this;
	}

	DataModelBuilder& DataModelBuilder::createDO(const QString &t_name)
	{
		m_lastDO = QSharedPointer<DataObject>::create(m_lastLN.get(), t_name);
		m_lastLN->addChild(m_lastDO);
		return *this;
	}

	DataModelBuilder& DataModelBuilder::createDA(const QString &t_name,
												const QString &t_fc, int t_fcNum)
	{
		m_lastDA = QSharedPointer<DataAttribute>::create(m_lastDO.get(), t_name, t_fc, t_fcNum);
		m_lastDO->addChild(m_lastDA);
		return *this;
	}

	DataModelBuilder& DataModelBuilder::createSDA(QSharedPointer<Item> t_parent, const QString &t_name)
	{
		m_lastSDA = QSharedPointer<SubAttribute>::create(t_parent.get(), t_name);
		t_parent->addChild(m_lastSDA);
		return *this;
	}

	QSharedPointer<DataModel> DataModelBuilder::build()
	{
		m_model->findModelName();

		for (size_t i=0;i<m_model->getChildCount();i++) {
			auto ld = m_model->getLogicalDevice(i);

			for (size_t j=0;j<ld->getChildCount();j++) {
				auto ln = ld->getChild<LogicalNode>(j);

				auto doTable = LN_FlatBuilder::create(ln);
				ln->setDO_Table(doTable);
			}
		}
		return m_model;
	}
}