#include <gtest/gtest.h>

#include "core/data_model_builder.hpp"

namespace UnitTests
{
    // ─── ReportBlock ────────────────────────────────────────────────

    TEST(ReportBlock, Defaults)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createRCB("brcbAnalog", "IEDNameA_LD_1/LLN0", true);

        auto rcb = builder.lastRCB();
        ASSERT_NE(rcb, nullptr);

        EXPECT_EQ(rcb->getName(), "brcbAnalog");
        EXPECT_EQ(rcb->lnRef(), "IEDNameA_LD_1/LLN0");
        EXPECT_TRUE(rcb->isBuffered());
        EXPECT_FALSE(rcb->rptEna());
        EXPECT_FALSE(rcb->resv());
        EXPECT_EQ(rcb->rptId(), "");
        EXPECT_EQ(rcb->dsRef(), "");
        EXPECT_EQ(rcb->trgOps(), 0);
        EXPECT_EQ(rcb->confRev(), 0u);
        EXPECT_EQ(rcb->bufTm(), 0u);
        EXPECT_EQ(rcb->intgPd(), 0u);
        EXPECT_EQ(rcb->owner(), "");
    }

    TEST(ReportBlock, SettersAndGetters)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createRCB("brcbAnalog", "IEDNameA_LD_1/LLN0", true);

        auto rcb = builder.lastRCB();

        rcb->setRptEna(true);
        rcb->setResv(true);
        rcb->setRptId("reportId_01");
        rcb->setDsRef("IEDNameA_LD_1/LLN0.dsAnalog");
        rcb->setTrgOps(0x1F);
        rcb->setConfRev(42);
        rcb->setBufTm(500);
        rcb->setIntgPd(2000);
        rcb->setOwner("testOwner");

        EXPECT_TRUE(rcb->rptEna());
        EXPECT_TRUE(rcb->resv());
        EXPECT_EQ(rcb->rptId(), "reportId_01");
        EXPECT_EQ(rcb->dsRef(), "IEDNameA_LD_1/LLN0.dsAnalog");
        EXPECT_EQ(rcb->trgOps(), 0x1F);
        EXPECT_EQ(rcb->confRev(), 42u);
        EXPECT_EQ(rcb->bufTm(), 500u);
        EXPECT_EQ(rcb->intgPd(), 2000u);
        EXPECT_EQ(rcb->owner(), "testOwner");
    }

    TEST(ReportBlock, BufferedVsUnbuffered)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createRCB("brcb01", "IEDNameA_LD_1/LLN0", true)
               .createRCB("urcb01", "IEDNameA_LD_1/LLN0", false);

        auto model = builder.build(nullptr);
        auto rcbList = model->getReportCBList();
        ASSERT_EQ(rcbList.size(), 2);

        EXPECT_TRUE(rcbList[0]->isBuffered());
        EXPECT_FALSE(rcbList[1]->isBuffered());
    }

    // ─── GooseControlBlock ──────────────────────────────────────────

    TEST(GooseControlBlock, Defaults)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createGOCB("gcbAnalog", "IEDNameA_LD_1/LLN0");

        auto gocb = builder.lastGOCB();
        ASSERT_NE(gocb, nullptr);

        EXPECT_EQ(gocb->getName(), "gcbAnalog");
        EXPECT_EQ(gocb->lnRef(), "IEDNameA_LD_1/LLN0");
        EXPECT_FALSE(gocb->goEna());
        EXPECT_EQ(gocb->goId(), "");
        EXPECT_EQ(gocb->datSet(), "");
        EXPECT_EQ(gocb->confRev(), 0u);
        EXPECT_EQ(gocb->minTime(), 0u);
        EXPECT_EQ(gocb->maxTime(), 0u);
        EXPECT_EQ(gocb->appId(), 0);
        EXPECT_EQ(gocb->vlanId(), 0);
        EXPECT_EQ(gocb->vlanPriority(), 0);
    }

    TEST(GooseControlBlock, SettersAndGetters)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createGOCB("gcbAnalog", "IEDNameA_LD_1/LLN0");

        auto gocb = builder.lastGOCB();

        gocb->setGoEna(true);
        gocb->setGoId("goose_001");
        gocb->setDatSet("IEDNameA_LD_1/LLN0.dsAnalog");
        gocb->setConfRev(5);
        gocb->setMinTime(100);
        gocb->setMaxTime(5000);
        gocb->setAppId(0x1234);
        gocb->setVlanId(100);
        gocb->setVlanPriority(4);

        EXPECT_TRUE(gocb->goEna());
        EXPECT_EQ(gocb->goId(), "goose_001");
        EXPECT_EQ(gocb->datSet(), "IEDNameA_LD_1/LLN0.dsAnalog");
        EXPECT_EQ(gocb->confRev(), 5u);
        EXPECT_EQ(gocb->minTime(), 100u);
        EXPECT_EQ(gocb->maxTime(), 5000u);
        EXPECT_EQ(gocb->appId(), 0x1234);
        EXPECT_EQ(gocb->vlanId(), 100);
        EXPECT_EQ(gocb->vlanPriority(), 4);
    }

    // ─── SV_ControlBlock ────────────────────────────────────────────

    TEST(SV_ControlBlock, Defaults)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createSVCB("msvcb01", "IEDNameA_LD_1/LLN0", true);

        auto svcb = builder.lastSVCB();
        ASSERT_NE(svcb, nullptr);

        EXPECT_EQ(svcb->getName(), "msvcb01");
        EXPECT_EQ(svcb->lnRef(), "IEDNameA_LD_1/LLN0");
        EXPECT_TRUE(svcb->isMulticast());
        EXPECT_FALSE(svcb->svEna());
        EXPECT_EQ(svcb->svId(), "");
        EXPECT_EQ(svcb->datSet(), "");
        EXPECT_EQ(svcb->confRev(), 0u);
        EXPECT_EQ(svcb->smpRate(), 0u);
        EXPECT_EQ(svcb->noASDU(), 0);
    }

    TEST(SV_ControlBlock, SettersAndGetters)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createSVCB("msvcb01", "IEDNameA_LD_1/LLN0", true);

        auto svcb = builder.lastSVCB();

        svcb->setSvEna(true);
        svcb->setSvId("sv_stream_01");
        svcb->setDatSet("IEDNameA_LD_1/LLN0.dsSV");
        svcb->setConfRev(3);
        svcb->setSmpRate(4000);
        svcb->setNoASDU(2);

        EXPECT_TRUE(svcb->svEna());
        EXPECT_EQ(svcb->svId(), "sv_stream_01");
        EXPECT_EQ(svcb->datSet(), "IEDNameA_LD_1/LLN0.dsSV");
        EXPECT_EQ(svcb->confRev(), 3u);
        EXPECT_EQ(svcb->smpRate(), 4000u);
        EXPECT_EQ(svcb->noASDU(), 2);
    }

    TEST(SV_ControlBlock, MulticastVsUnicast)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createSVCB("msvcb01", "IEDNameA_LD_1/LLN0", true)
               .createSVCB("usvcb01", "IEDNameA_LD_1/LLN0", false);

        auto model = builder.build(nullptr);
        auto svList = model->getSV_CBList();
        ASSERT_EQ(svList.size(), 2);

        EXPECT_TRUE(svList[0]->isMulticast());
        EXPECT_FALSE(svList[1]->isMulticast());
    }

    // ─── DataSet & DataSetItem ──────────────────────────────────────

    TEST(DataSet, ItemCount)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
                   .createDO("Mod")
                   .createDO("Beh")
                   .createDO("Health")
               .createDataSet("dsAnalog", "IEDNameA_LD_1/LLN0", false)
                   .createDataSet_Elem("IEDNameA_LD_1/LLN0.Mod", "ST")
                   .createDataSet_Elem("IEDNameA_LD_1/LLN0.Beh", "ST")
                   .createDataSet_Elem("IEDNameA_LD_1/LLN0.Health", "ST");

        auto ds = builder.lastDataSet();
        ASSERT_NE(ds, nullptr);

        EXPECT_EQ(ds->getName(), "dsAnalog");
        EXPECT_EQ(ds->ref(), "IEDNameA_LD_1/LLN0");
        EXPECT_EQ(static_cast<int>(ds->getItemCount()), 3);
    }

    TEST(DataSet, ItemRefAndFc)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
                   .createDO("Mod")
               .createDataSet("dsAnalog", "IEDNameA_LD_1/LLN0", false)
                   .createDataSet_Elem("IEDNameA_LD_1/LLN0.Mod", "ST");

        auto ds = builder.lastDataSet();
        ASSERT_EQ(static_cast<int>(ds->getItemCount()), 1);

        auto item = ds->getItem(0).staticCast<Core::DataSetItem>();
        ASSERT_NE(item, nullptr);
        EXPECT_EQ(item->ref(), "IEDNameA_LD_1/LLN0.Mod");
        EXPECT_EQ(item->fc(), "ST");
    }

    TEST(DataSet, PushedToModel)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
                   .createDO("Mod")
               .createDataSet("ds1", "IEDNameA_LD_1/LLN0", false)
                   .createDataSet_Elem("IEDNameA_LD_1/LLN0.Mod", "ST")
               .createDataSet("ds2", "IEDNameA_LD_1/LLN0", false)
                   .createDataSet_Elem("IEDNameA_LD_1/LLN0.Mod", "MX");

        auto model = builder.build(nullptr);
        EXPECT_EQ(model->getDataSetList().size(), 2);
    }
}
