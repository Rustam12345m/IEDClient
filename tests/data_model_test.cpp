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

#include "data_model_test.hpp"

#include <iostream>

namespace CoreTests
{
    void DataModel_Fixt::SetUp()
    {
        Core::DataModelBuilder builder;

        builder.createLD("IEDNameA_LD_1")
                .createLN("LLN0")
                    .createDO("NamPlt")
                        .createDA("vendor", "")
                        .createDA("swRev", "")
                        .createDA("d", "")
                        .createDA("configRev", "")
                        .createDA("ldNs", "")
                .createLN("LPHD1")
                    .createDO("PhyNam")
                        .createDA("vendor", "")
                        .createDA("hwRev", "")
                        .createDA("swRev", "")
                        .createDA("serNum", "")
                        .createDA("model", "")
                .createLN("GGIO1")
                    .createDO("AnIn1")
                        .createDA("mag", "MX")
                            .createSDA(builder.lastDA(), "f")
                    .createDO("Ind1")
                        .createDA("stVal", "ST")
            .createLD("IEDNameB_LD_2")
                .createLN("LLN0")
                    .createDO("NamPlt")
                        .createDA("vendor", "")
                        .createDA("swRev", "")
                        .createDA("d", "")
                        .createDA("configRev", "")
                        .createDA("ldNs", "")
                .createLN("LPHD1")
                    .createDO("PhyNam")
                        .createDA("vendor", "")
                        .createDA("hwRev", "")
                        .createDA("swRev", "")
                        .createDA("serNum", "")
                        .createDA("model", "");

        m_model = builder.build(nullptr);
    }

    void DataModel_Fixt::TearDown()
    {
        m_model.reset();
    }


	TEST_F(DataModel_Fixt, Find_Items_By_Names) {
		ASSERT_EQ(m_model->getName(), "IEDName");

		// LLN0
		ASSERT_NE(m_model->findSubItem("A_LD_1"), nullptr);
		ASSERT_NE(m_model->findSubItem("A_LD_1", "LLN0"), nullptr);
		ASSERT_NE(m_model->findSubItem("A_LD_1", "LLN0", "NamPlt"), nullptr);
		ASSERT_NE(m_model->findSubItem("A_LD_1", "LLN0", "NamPlt", "vendor"), nullptr);
		ASSERT_NE(m_model->findSubItem("A_LD_1", "LLN0", "NamPlt", "ldNs"), nullptr);

		// LPHD1
		ASSERT_NE(m_model->findSubItem("B_LD_2"), nullptr);
		ASSERT_NE(m_model->findSubItem("B_LD_2", "LPHD1"), nullptr);
		ASSERT_NE(m_model->findSubItem("B_LD_2", "LPHD1", "PhyNam"), nullptr);
		ASSERT_NE(m_model->findSubItem("B_LD_2", "LPHD1", "PhyNam", "vendor"), nullptr);
		ASSERT_NE(m_model->findSubItem("B_LD_2", "LPHD1", "PhyNam", "model"), nullptr);
	}

	TEST_F(DataModel_Fixt, Find_Items_By_MMS_Ref) {
		ASSERT_EQ(m_model->getName(), "IEDName");

		// LLN0
		ASSERT_NE(m_model->getItemByReference("IEDNameA_LD_1/LLN0.NamPlt"), nullptr);
		ASSERT_NE(m_model->getItemByReference("IEDNameA_LD_1/LLN0.NamPlt.vendor"), nullptr);
		ASSERT_NE(m_model->getItemByReference("IEDNameA_LD_1/LLN0.NamPlt.ldNs"), nullptr);

		// LPHD1
		ASSERT_NE(m_model->getItemByReference("IEDNameB_LD_2/LPHD1.PhyNam"), nullptr);
		ASSERT_NE(m_model->getItemByReference("IEDNameB_LD_2/LPHD1.PhyNam.vendor"), nullptr);
		ASSERT_NE(m_model->getItemByReference("IEDNameB_LD_2/LPHD1.PhyNam.model"), nullptr);

        // GGIO1
		ASSERT_NE(m_model->getItemByReference("IEDNameA_LD_1/GGIO1.AnIn1.mag.f"), nullptr);
		ASSERT_NE(m_model->getItemByReference("IEDNameA_LD_1/GGIO1.Ind1.stVal"), nullptr);
	}

	TEST_F(DataModel_Fixt, SubAttr_And_DataAttr_Values) {
		// Analogue values
		{
			auto item = m_model->findSubItem("A_LD_1", "GGIO1", "AnIn1", "mag", "f");
			ASSERT_NE(item, nullptr);

			EXPECT_EQ(item->getValue(), "");

			item->updateValue(Core::ModelItemValue::create("3.1415"));
			EXPECT_EQ(item->getValue(), "3.1415") << "DA = " << item->getValue().toStdString();
		}

		// State signals
		{
			auto item = m_model->findSubItem("A_LD_1", "GGIO1", "Ind1", "stVal");
			ASSERT_NE(item, nullptr);

			item->updateValue(Core::ModelItemValue::create("1"));
			EXPECT_EQ(item->getValue(), "1");

			item->updateValue(Core::ModelItemValue::create("0"));
			EXPECT_EQ(item->getValue(), "0");
		}
	}

	TEST_F(DataModel_Fixt, Properties_From_LLN0_LPHD1) {
		// LLN0
		{
			auto namPlt = m_model->findSubItem("A_LD_1", "LLN0", "NamPlt");
			ASSERT_NE(namPlt, nullptr);

			auto vendor = namPlt->findSubItem("vendor");
			ASSERT_NE(vendor, nullptr);

			vendor->updateValue(Core::ModelItemValue::create("Test_Company_Name"));
			ASSERT_EQ(vendor->getValue(), "Test_Company_Name");
		}

		// LPHD1
		{
			auto phyNam = m_model->findSubItem("A_LD_1", "LLN0", "NamPlt");
			ASSERT_NE(phyNam, nullptr);

			auto vendor = phyNam->findSubItem("vendor");
			ASSERT_NE(vendor, nullptr);

			vendor->updateValue(Core::ModelItemValue::create("Test_Company_Name"));
			ASSERT_EQ(vendor->getValue(), "Test_Company_Name");
		}
	}

	TEST_F(DataModel_Fixt, Update_LNode) {

	}


	TEST(DataModelBuilder, DataModel_Creation_1) {
		Core::DataModelBuilder builder;

		builder.createLD("IEDNameA_LD_10")
					.createLN("LLN0")
						.createDO("Mod")
						.createDO("Beh")
						.createDO("Health");

		auto model = builder.build(nullptr);

		ASSERT_EQ(model->getName(), "IEDName");
	}

	TEST(DataModelBuilder, DataModel_Creation_2) {
		Core::DataModelBuilder builder;

		builder.createLD("IEDNameA_LD_1")
					.createLN("LLN0")
						.createDO("Mod")
						.createDO("Beh")
						.createDO("Health")
					.createLN("LPHD1")
						.createDO("Mod")
						.createDO("Beh")
						.createDO("Health")
				.createLD("IEDNameB_LD_2")
					.createLN("LLN0")
						.createDO("Mod")
						.createDO("Beh")
						.createDO("Health")
					.createLN("GGIO1")
						.createDO("AnIn1")
							.createDA("mag", "MX")
								.createSDA(builder.lastDA(), "f")
						.createDO("Ind1")
							.createDA("stVal", "ST");

		auto model = builder.build(nullptr);

		ASSERT_EQ(model->getName(), "IEDName");
	}

	TEST(DataModelBuilder, Find_Items_In_The_Tree) {
		Core::DataModelBuilder builder;

		builder.createLD("IEDNameA_LD_1")
					.createLN("LLN0")
						.createDO("NamPlt")
							.createDA("vendor", "")
							.createDA("swRev", "")
							.createDA("d", "")
							.createDA("configRev", "")
							.createDA("ldNs", "")
					.createLN("LPHD1")
						.createDO("PhyNam")
							.createDA("vendor", "")
							.createDA("hwRev", "")
							.createDA("swRev", "")
							.createDA("serNum", "")
							.createDA("model", "")
				.createLD("IEDNameB_LD_2")
					.createLN("LLN0")
						.createDO("NamPlt")
							.createDA("vendor", "")
							.createDA("swRev", "")
							.createDA("d", "")
							.createDA("configRev", "")
							.createDA("ldNs", "")
					.createLN("LPHD1")
						.createDO("PhyNam")
							.createDA("vendor", "")
							.createDA("hwRev", "")
							.createDA("swRev", "")
							.createDA("serNum", "")
							.createDA("model", "");

		auto model = builder.build(nullptr);

		ASSERT_EQ(model->getName(), "IEDName");

		// LLN0
		ASSERT_NE(model->findSubItem("A_LD_1"), nullptr);
		ASSERT_NE(model->findSubItem("A_LD_1", "LLN0"), nullptr);
		ASSERT_NE(model->findSubItem("A_LD_1", "LLN0", "NamPlt"), nullptr);
		ASSERT_NE(model->findSubItem("A_LD_1", "LLN0", "NamPlt", "vendor"), nullptr);
		ASSERT_NE(model->findSubItem("A_LD_1", "LLN0", "NamPlt", "ldNs"), nullptr);

		// LPHD1
		ASSERT_NE(model->findSubItem("B_LD_2"), nullptr);
		ASSERT_NE(model->findSubItem("B_LD_2", "LPHD1"), nullptr);
		ASSERT_NE(model->findSubItem("B_LD_2", "LPHD1", "PhyNam"), nullptr);
		ASSERT_NE(model->findSubItem("B_LD_2", "LPHD1", "PhyNam", "vendor"), nullptr);
		ASSERT_NE(model->findSubItem("B_LD_2", "LPHD1", "PhyNam", "model"), nullptr);
	}

	TEST(DataModelBuilder, DataSet_Creation) {
		Core::DataModelBuilder builder;

		builder.createLD("IEDNameA_LD_10")
					.createLN("LLN0")
						.createDO("Mod")
						.createDO("Beh")
						.createDO("Health")
					.createDataSet("DataSet1", "IEDNameA_LD_10/LLN0", false)
						.createDataSet_Elem("IEDNameA_LD_10/LLN0.Mod", "ST")
						.createDataSet_Elem("IEDNameA_LD_10/LLN0.Beh", "ST")
						.createDataSet_Elem("IEDNameA_LD_10/LLN0.Health", "ST");

		auto model = builder.build(nullptr);

		ASSERT_EQ(model->getName(), "IEDName");
	}

    TEST(DataModelBuilder, DataSet_GetItems) {
		Core::DataModelBuilder builder;

		builder.createLD("IEDNameA_LD_10")
					.createLN("LLN0")
						.createDO("Mod")
						.createDO("Beh")
						.createDO("Health")
					.createDataSet("DataSet1", "IEDNameA_LD_10/LLN0", false)
						.createDataSet_Elem("IEDNameA_LD_10/LLN0.Mod", "ST")
						.createDataSet_Elem("IEDNameA_LD_10/LLN0.Beh", "ST")
						.createDataSet_Elem("IEDNameA_LD_10/LLN0.Health", "ST");

		auto model = builder.build(nullptr);

		ASSERT_EQ(model->getName(), "IEDName");
	}
}