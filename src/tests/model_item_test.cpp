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

#include <gtest/gtest.h>

#include "core/data_model_builder.hpp"
#include "core/item_factory.hpp"

namespace UnitTests
{
    TEST(DataModel, DataModel)
    {
        auto model = Core::ItemFactory::createModel("NoName").staticCast<Core::DataModel>();

        auto ld1 = Core::ItemFactory::createLD(model.get(), "IEDNameLD1");
        auto ld2 = Core::ItemFactory::createLD(model.get(), "IEDNameLD2");
    }

    TEST(DataModel, LogicalDevices)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
                   .createLN("LLN0")
               .createLD("IEDNameB_LD_2")
                   .createLN("LLN0");

        auto model = builder.build(nullptr);

        ASSERT_EQ(static_cast<int>(model->getItemCount()), 2);

        auto ld0 = model->getLogicalDevice(0);
        auto ld1 = model->getLogicalDevice(1);
        ASSERT_NE(ld0, nullptr);
        ASSERT_NE(ld1, nullptr);

        EXPECT_EQ(ld0->getName(), "A_LD_1");
        EXPECT_EQ(ld1->getName(), "B_LD_2");
    }

    TEST(DataModel, LogicalNodes)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
                   .createLN("LLN0")
                       .createDO("NamPlt")
                   .createLN("LPHD1")
                       .createDO("PhyNam")
                   .createLN("GGIO1")
                       .createDO("AnIn1");

        auto model = builder.build(nullptr);
        auto ld = model->getLogicalDevice(0);
        ASSERT_NE(ld, nullptr);

        // LD should have 3 LNs
        EXPECT_EQ(static_cast<int>(ld->getItemCount()), 3);

        // Special accessors for LLN0 and LPHD1
        ASSERT_NE(ld->lln0(), nullptr);
        ASSERT_NE(ld->lphd1(), nullptr);
        EXPECT_EQ(ld->lln0()->getName(), "LLN0");
        EXPECT_EQ(ld->lphd1()->getName(), "LPHD1");

        // Access LN by index
        auto ln2 = model->getLogicalNode(0, 2);
        ASSERT_NE(ln2, nullptr);
        EXPECT_EQ(ln2->getName(), "GGIO1");
    }

    TEST(DataModel, DataObjects)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
                   .createLN("LLN0")
                       .createDO("Mod")
                       .createDO("Beh")
                       .createDO("Health");

        auto model = builder.build(nullptr);
        auto ln = model->getLogicalNode(0, 0);
        ASSERT_NE(ln, nullptr);

        EXPECT_EQ(static_cast<int>(ln->getItemCount()), 3);

        auto mod = ln->findSubItem("Mod");
        ASSERT_NE(mod, nullptr);
        EXPECT_EQ(mod->getName(), "Mod");
        EXPECT_EQ(mod->getParent(), ln.get());
    }

    TEST(DataModel, DataSets)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
                   .createLN("LLN0")
                       .createDO("Mod")
                       .createDO("Beh")
               .createDataSet("ds1", "IEDNameA_LD_1/LLN0", false)
                   .createDataSet_Elem("IEDNameA_LD_1/LLN0.Mod", "ST")
                   .createDataSet_Elem("IEDNameA_LD_1/LLN0.Beh", "ST")
               .createLD("IEDNameB_LD_2")
                   .createLN("LLN0")
                       .createDO("Ind1")
               .createDataSet("ds2", "IEDNameB_LD_2/LLN0", false)
                   .createDataSet_Elem("IEDNameB_LD_2/LLN0.Ind1", "MX");

        auto model = builder.build(nullptr);
        auto dsList = model->getDataSetList();
        ASSERT_EQ(dsList.size(), 2);

        EXPECT_EQ(dsList[0]->getName(), "ds1");
        EXPECT_EQ(dsList[0]->ref(), "IEDNameA_LD_1/LLN0");
        EXPECT_EQ(static_cast<int>(dsList[0]->getItemCount()), 2);

        EXPECT_EQ(dsList[1]->getName(), "ds2");
        EXPECT_EQ(dsList[1]->ref(), "IEDNameB_LD_2/LLN0");
        EXPECT_EQ(static_cast<int>(dsList[1]->getItemCount()), 1);
    }
}
