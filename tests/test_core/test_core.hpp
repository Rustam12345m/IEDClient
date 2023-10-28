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

#include <gtest/gtest.h>
#include "core/item_factory.hpp"
#include "core/data_model_builder.hpp"

namespace CoreTests
{
	/*
	class ModelDataTest : public ::testing::Test
	{
	protected:
		ModelDataTest() {
		}
		~ModelDataTest() override {
		}

		void SetUp() override {
		}
		void TearDown() override {
		}
	};
	TEST_F(ModelDataTest, SubTest1) {
	}
	TEST_F(ModelDataTest, SubTest2) {
	}
	*/

	TEST(DataModelBuilder, Simple) {
		Core::DataModelBuilder builder;

		builder.createLD("IEDNameA_LD_1")
					.createLN("LLN0")
						.createDO("Mod")
						.createDO("Beh")
						.createDO("Health");

		auto model = builder.build();

		ASSERT_EQ(model->name(), "IEDName");
	}

	TEST(DataModelBuilder, Simple2) {
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
							.createSDA(builder.lastDO(), "mag")
								.createSDA(builder.lastDA(), "f")
						.createDO("Ind1")
							.createSDA(builder.lastDO(), "stVal");

		auto model = builder.build();

		ASSERT_EQ(model->name(), "IEDName");
	}

	TEST(DataModel, DataModel) {
		auto model = Core::ItemFactory::createModel("NoName").staticCast<Core::DataModel>();

		auto ld1 = Core::ItemFactory::createLD(model.get(), "IEDNameLD1");
		auto ld2 = Core::ItemFactory::createLD(model.get(), "IEDNameLD2");

		model->findModelName();

		ASSERT_EQ(model->name(), "IEDName");
	}

	TEST(DataModel, LogicalDevices) {

	}

	TEST(DataModel, LogicalNodes) {
		
	}

	TEST(DataModel, DataObjects) {
		
	}

	TEST(DataModel, DataSets) {
		
	}
}