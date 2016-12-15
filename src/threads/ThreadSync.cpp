#include "ThreadSync.h"

#include "../sync/PullTable.h"
#include "../classes/AppBill.h"

void ThreadSync::run() {

    manager->pull();
}

ThreadSync::ThreadSync() {

    id = idName();
    name = "Sync";
    manager = ManagerPull::instance();

    for (auto syncPar : app().conf.sync) {

        manager->add(new PullTable(syncPar.src, syncPar.dst, syncPar.key, syncPar.event));
    }

//    manager->add(new PullClient());
//    manager->add(new PullOrganization());
//    manager->add(new PullGeo());
//    manager->add(new PullMobPrefix());
//    manager->add(new PullGeoPrefix());
//    manager->add(new PullInstance());
//    manager->add(new PullNetworkPrefix());
//    manager->add(new PullOperator());
//    manager->add(new PullPricelist());
//    manager->add(new PullTariff());
//    manager->add(new PullTariffPackage());
//    manager->add(new PullDefs());
//    manager->add(new PullServiceNumber());
//    manager->add(new PullServicePackage());
//    manager->add(new PullServiceTrunk());
//    manager->add(new PullServiceTrunkSettings());
//    manager->add(new PullLogTariff());
//    manager->add(new PullStatPrefixlist());
//    manager->add(new PullStatDestinationPrefixlists());
//    manager->add(new PullCurrencyRate());
//    manager->add(new PullServer());
//    manager->add(new PullHub());
//    manager->add(new PullAirp());
//    manager->add(new PullNumber());
//    manager->add(new PullOutcome());
//    manager->add(new PullPrefixlist());
//    manager->add(new PullPrefixlistPrefix());
//    manager->add(new PullReleaseReason());
//    manager->add(new PullRouteCase());
//    manager->add(new PullRouteTable());
//    manager->add(new PullRouteTableRoute());
//    manager->add(new PullRoutingReportData());
//    manager->add(new PullTrunk());
//    manager->add(new PullTrunkGroup());
//    manager->add(new PullTrunkGroupItem());
//    manager->add(new PullTrunkNumberPreprocessing());
//    manager->add(new PullTrunkPriority());
//    manager->add(new PullTrunkRule());
//    manager->add(new PullTrunkTrunkRule());
//
//    manager->add(new NNPAccountTariffLightPull());
//    manager->add(new NNPNumberRangePrefixPull());
//    manager->add(new NNPOperatorPull());
//    manager->add(new NNPPackagePricelistPull());
//    manager->add(new NNPPackagePull());
//    manager->add(new NNPPrefixPull());
//    manager->add(new NNPDestinationPull());
//    manager->add(new NNPNumberRangePull());
//    manager->add(new NNPPackageMinutePull());
//    manager->add(new NNPPackagePricePull());
//    manager->add(new NNPPrefixDestinatioPull());
//    manager->add(new NNPRegionPull());
}
