/*
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
		m_model->push(m_lastLD);
		return *this;
	}

	DataModelBuilder& DataModelBuilder::createLN(const QString &t_name)
	{
		m_lastLN = QSharedPointer<LogicalNode>::create(m_lastLD.get(), t_name);
		m_lastLD->push(m_lastLN);
		return *this;
	}

	DataModelBuilder& DataModelBuilder::createDO(const QString &t_name)
	{
		m_lastDO = QSharedPointer<DataObject>::create(m_lastLN.get(), t_name);
		m_lastLN->push(m_lastDO);
		return *this;
	}

	DataModelBuilder& DataModelBuilder::createDA(const QString &t_name,
												const QString &t_fc, int t_fcNum)
	{
		m_lastDA = QSharedPointer<DataAttribute>::create(m_lastDO.get(), t_name, t_fc, t_fcNum);
		m_lastDO->push(m_lastDA);
		return *this;
	}

	DataModelBuilder& DataModelBuilder::createSDA(QSharedPointer<Item> t_parent, const QString &t_name)
	{
		m_lastSDA = QSharedPointer<SubAttribute>::create(t_parent.get(), t_name);
		t_parent->push(m_lastSDA);
		return *this;
	}

	DataModelBuilder& DataModelBuilder::createDataSet(const QString &t_name, const QString &t_ref, bool t_del)
	{
		m_lastDataSet = QSharedPointer<DataSet>::create(m_model.get(), t_name, t_ref, t_del);
		m_model->pushDataSet(m_lastDataSet);
		return *this;
	}

	DataModelBuilder& DataModelBuilder::createDataSet_Elem(const QString &t_ref, const QString &t_fc)
	{
		auto dsEnt = QSharedPointer<DataSetEntity>::create(lastDataSet().get(), t_ref, t_fc);
		lastDataSet()->push(dsEnt);
		return *this;
	}

	QSharedPointer<DataModel> DataModelBuilder::build()
	{
		m_model->calcIEDNameFromLD();

		for (size_t i=0;i<m_model->getItemCount();i++) {
			auto ld = m_model->getLogicalDevice(i);

			for (size_t j=0;j<ld->getItemCount();j++) {
				auto ln = ld->getItem<LogicalNode>(j);

				ln->m_doTable = LN_StateTableBuilder::create(ln);
			}
		}
		return m_model;
	}
}