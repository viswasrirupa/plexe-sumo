//---------------------------------------------------------------------------//
//                        GUINet.cpp -
//  A MSNet extended by some values for usage within the gui
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                : Sept 2002
//  copyright            : (C) 2002 by Daniel Krajzewicz
//  organisation         : IVF/DLR http://ivf.dlr.de
//  email                : Daniel.Krajzewicz@dlr.de
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//---------------------------------------------------------------------------//
namespace
{
    const char rcsid[] =
    "$Id$";
}
// $Log$
// Revision 1.35  2004/08/02 11:58:00  dkrajzew
// using OutputDevices instead of ostreams
//
// Revision 1.34  2004/07/02 08:57:29  dkrajzew
// using global object selection; detector handling (handling of additional elements) revisited
//
// Revision 1.33  2004/04/02 11:17:07  dkrajzew
// simulation-wide output files are now handled by MSNet directly
//
// Revision 1.32  2004/03/19 12:57:55  dkrajzew
// porting to FOX
//
// Revision 1.31  2004/02/10 07:07:13  dkrajzew
// debugging of network loading after a network failed to be loaded;
//  memory leaks removal
//
// Revision 1.30  2004/02/05 16:30:59  dkrajzew
// multiplicate deletion of E3-detectors on application quit patched
//
// Revision 1.29  2004/01/26 06:59:38  dkrajzew
// work on detectors: e3-detectors loading and visualisation;
//  variable offsets and lengths for lsa-detectors;
//  coupling of detectors to tl-logics; different detector
//  visualistaion in dependence to his controller
//
// Revision 1.28  2003/12/11 06:24:55  dkrajzew
// implemented MSVehicleControl as the instance responsible for vehicles
//
// Revision 1.27  2003/12/09 11:28:23  dkrajzew
// removed some memory leaks
//
// Revision 1.26  2003/12/04 13:36:52  dkrajzew
// detector name changing applied
//
// Revision 1.25  2003/11/26 09:48:58  dkrajzew
// time display added to the tl-logic visualisation
//
// Revision 1.24  2003/11/20 13:05:32  dkrajzew
// loading and using of predefined vehicle colors added
//
// Revision 1.23  2003/11/18 14:30:40  dkrajzew
// debugged and completed lane merging detectors
//
// Revision 1.22  2003/11/12 14:01:54  dkrajzew
// visualisation of tl-logics added
//
// Revision 1.21  2003/11/11 08:13:23  dkrajzew
// consequent usage of Position2D instead of two doubles
//
// Revision 1.20  2003/10/30 08:59:43  dkrajzew
// first implementation of aggregated views using E2-detectors
//
// Revision 1.19  2003/10/22 15:42:56  dkrajzew
// we have to distinct between two teleporter versions now
//
// Revision 1.18  2003/10/02 14:51:20  dkrajzew
// visualisation of E2-detectors implemented
//
// Revision 1.17  2003/09/22 12:38:51  dkrajzew
// detectors need const Lanes
//
// Revision 1.16  2003/09/05 15:01:24  dkrajzew
// first tries for an implementation of aggregated views
//
// Revision 1.15  2003/08/04 11:35:51  dkrajzew
// only GUIVehicles need a color definition; process of building cars changed
//
// Revision 1.14  2003/07/30 08:54:14  dkrajzew
// the network is capable to display the networks state, now
//
// Revision 1.13  2003/07/22 14:59:27  dkrajzew
// changes due to new detector handling
//
// Revision 1.12  2003/07/16 15:24:55  dkrajzew
// GUIGrid now handles the set of things to draw in another manner than
//  GUIEdgeGrid did; Further things to draw implemented
//
// Revision 1.11  2003/06/05 06:29:50  dkrajzew
// first tries to build under linux: warnings removed; moc-files included
//  Makefiles added
//
// Revision 1.10  2003/05/28 07:52:31  dkrajzew
// new usage of MSEventControl adapted
//
// Revision 1.9  2003/05/21 15:15:41  dkrajzew
// yellow lights implemented (vehicle movements debugged
//
// Revision 1.8  2003/05/20 09:26:57  dkrajzew
// data retrieval for new views added
//
// Revision 1.7  2003/04/16 09:50:06  dkrajzew
// centering of the network debugged; additional parameter of maximum display
//  size added
//
// Revision 1.6  2003/04/14 08:27:17  dkrajzew
// new globject concept implemented
//
// Revision 1.5  2003/03/20 16:19:28  dkrajzew
// windows eol removed; multiple vehicle emission added
//
// Revision 1.4  2003/03/12 16:52:06  dkrajzew
// centering of objects debuggt
//
// Revision 1.3  2003/02/07 10:39:17  dkrajzew
// updated
//
/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <utility>
#include <set>
#include <microsim/MSNet.h>
#include <microsim/MSJunction.h>
#include <microsim/MSInductLoop.h>
#include <microsim/MSDetectorSubSys.h>
#include <microsim/MSEdge.h>
#include <microsim/MSVehicleTransfer.h>
#include <microsim/MSVehicle.h>
#include <microsim/MSEmitControl.h>
#include <microsim/MSTrafficLightLogic.h>
#include <utils/common/NamedObjectContSingleton.h>
#include <gui/GUIGlObjectStorage.h>
#include <utils/gfx/RGBColor.h>
#include "GUINetWrapper.h"
#include <guisim/guilogging/GLObjectValuePassConnector.h>
#include <guisim/GUIEdge.h>
#include <gui/GUIExcp_VehicleIsInvisible.h>
//#include <guisim/GUIEmitterWrapper.h>
#include <guisim/GUILaneSpeedTrigger.h>
#include <guisim/GUIDetectorWrapper.h>
#include <guisim/GUI_E2_ZS_Collector.h>
#include <guisim/GUI_E2_ZS_CollectorOverLanes.h>
#include <guisim/GUIE3Collector.h>
#include <guisim/GUITrafficLightLogicWrapper.h>
#include <guisim/GUILaneStateReporter.h>
#include <guisim/GUIJunctionWrapper.h>
#include <guisim/GUIVehicleControl.h>
#include <gui/GUIGlobals.h>
#include <microsim/MSLaneState.h>
#include <microsim/MSUpdateEachTimestepContainer.h>
#include "GUIVehicle.h"
#include "GUINet.h"
#include "GUIHelpingJunction.h"


/* =========================================================================
 * member method definitions
 * ======================================================================= */
GUINet::GUINet()
    : MSNet(), _grid(*this, 10, 10),
    myWrapper(new GUINetWrapper(gIDStorage, *this))
{
}


GUINet::~GUINet()
{
    gIDStorage.clear();
    // delete allocated wrappers
        // of junctions
    for(std::vector<GUIJunctionWrapper*>::iterator i1=myJunctionWrapper.begin(); i1!=myJunctionWrapper.end(); i1++) {
        delete (*i1);
    }
        // of addition structures
    GUIGlObject_AbstractAdd::clearDictionary();
        // of tl-logics
    typedef std::map<MSLink*, GUITrafficLightLogicWrapper*> Link2LogicMap;
    typedef std::set<GUITrafficLightLogicWrapper*> LogicSet;
    LogicSet known;
    for(Link2LogicMap::iterator i3=myLinks2Logic.begin(); i3!=myLinks2Logic.end(); i3++) {
        known.insert((*i3).second);
    }
    for(LogicSet::iterator i4=known.begin(); i4!=known.end(); i4++) {
        delete (*i4);
    }
        // of the network itself
    delete myWrapper;
    GUIE3Collector::clearInstances();
    // remove further gui-structures
//    MSUpdateEachTimestepContainer<MSUpdateEachTimestep<GUILaneStateReporter> >::getInstance()->clear();
//    MSUpdateEachTimestepContainer<MSUpdateEachTimestep<GLObjectValuePassConnector<double> > >::getInstance()->clear();
//    MSUpdateEachTimestepContainer<MSUpdateEachTimestep<GLObjectValuePassConnector<CompletePhaseDef> > >::getInstance()->clear();

}


const Boundery &
GUINet::getBoundery() const
{
    return _boundery;
}


void
GUINet::preInitGUINet( MSNet::Time startTimeStep,
                      MSVehicleControl *vc,
                      TimeVector dumpMeanDataIntervalls,
                      std::string baseNameDumpFiles )
{
    myInstance = new GUINet();
    MSNet::preInit(startTimeStep, vc,
        dumpMeanDataIntervalls, baseNameDumpFiles);
}


void
GUINet::initGUINet(std::string id, MSEdgeControl* ec,
                   MSJunctionControl* jc,
                   MSRouteLoaderControl *rlc,
                   MSTLLogicControl *tlc,
                   const std::vector<OutputDevice*> &streams)
{
    MSNet::init(id, ec, jc, rlc, tlc, streams);
    GUINet *net = static_cast<GUINet*>(MSNet::getInstance());
    // initialise edge storage for gui
    GUIEdge::fill(net->myEdgeWrapper);
    // initialise junction storage for gui
    GUIHelpingJunction::fill(net->myJunctionWrapper, gIDStorage);
    // initialise detector storage for gui
    initDetectors();
    // initialise the tl-map
    initTLMap();
    // build the grid
    net->_grid.init();
    // get the boundery
    net->_boundery = net->_grid.getBoundery();
}


void
GUINet::initDetectors()
{
    GUINet *net = static_cast<GUINet*>(MSNet::getInstance());
    // e2-detectors
    MSDetectorSubSys::E2Dict::ValueVector loopVec2(
        MSDetectorSubSys::E2Dict::getInstance()->getStdVector() );
//    net->myAdditionalWrapper.reserve(loopVec2.size()+net->myAdditionalWrapper.size());
    for(MSDetectorSubSys::E2Dict::ValueVector::iterator
        i2=loopVec2.begin(); i2!=loopVec2.end(); i2++) {
        const MSLane *lane = (*i2)->getLane();
        GUIEdge *edge =
            static_cast<GUIEdge*>(MSEdge::dictionary(lane->edge().id()));
        // build the wrapper
        if( (*i2)->getUsageType()==DU_SUMO_INTERNAL
            ||
            (*i2)->getUsageType()==DU_TL_CONTROL) {
            continue;
        }
        GUIDetectorWrapper *wrapper =
            static_cast<GUI_E2_ZS_Collector*>(*i2)->buildDetectorWrapper(
                gIDStorage, edge->getLaneGeometry(lane));
        // add to list
//        net->myAdditionalWrapper.push_back(wrapper);
        // add to dictionary
        net->myDetectorDict[wrapper->microsimID()] = wrapper;
    }
    // e2 over lanes -detectors
    MSDetectorSubSys::E2ZSOLDict::ValueVector loopVec3(
        MSDetectorSubSys::E2ZSOLDict::getInstance()->getStdVector() );
//    net->myAdditionalWrapper.reserve(loopVec2.size()+net->myAdditionalWrapper.size());
    for(MSDetectorSubSys::E2ZSOLDict::ValueVector::iterator
        i3=loopVec3.begin(); i3!=loopVec3.end(); i3++) {
        // build the wrapper
        GUIDetectorWrapper *wrapper =
            static_cast<GUI_E2_ZS_CollectorOverLanes*>(*i3)->buildDetectorWrapper(
                gIDStorage);
        // add to list
//        net->myAdditionalWrapper.push_back(wrapper);
        // add to dictionary
        net->myDetectorDict[wrapper->microsimID()] = wrapper;
    }
    // e1-detectors
    MSDetectorSubSys::LoopDict::ValueVector loopVec(
        MSDetectorSubSys::LoopDict::getInstance()->getStdVector() );
//    net->myAdditionalWrapper.reserve(loopVec.size()+net->myAdditionalWrapper.size());
    for(MSDetectorSubSys::LoopDict::ValueVector::iterator
        i=loopVec.begin(); i!=loopVec.end(); i++) {
        const MSLane *lane = (*i)->getLane();
        GUIEdge *edge =
            static_cast<GUIEdge*>(MSEdge::dictionary(lane->edge().id()));
        // build the wrapper
        GUIDetectorWrapper *wrapper =
            (*i)->buildDetectorWrapper(
                gIDStorage, edge->getLaneGeometry(lane));
        // add to list
//        net->myAdditionalWrapper.push_back(wrapper);
        // add to dictionary
        net->myDetectorDict[wrapper->microsimID()] = wrapper;
    }
    // e3-detectors
    const GUIE3Collector::InstanceVector &loopVec4 = GUIE3Collector::getInstances();
//    net->myAdditionalWrapper.reserve(loopVec4.size()+net->myAdditionalWrapper.size());
    for(GUIE3Collector::InstanceVector::const_iterator i4=loopVec4.begin(); i4!=loopVec4.end(); i4++) {
        // build the wrapper
        GUIDetectorWrapper *wrapper =
            (*i4)->buildDetectorWrapper(gIDStorage);
        // add to list
//        net->myAdditionalWrapper.push_back(wrapper);
        // add to dictionary
        net->myDetectorDict[wrapper->microsimID()] = wrapper;
    }
}


void
GUINet::initTLMap()
{
    GUINet *net = static_cast<GUINet*>(MSNet::getInstance());
    //
    typedef std::vector<MSTrafficLightLogic*> LogicVector;
    // get the list of loaded tl-logics
    LogicVector tlls = MSTrafficLightLogic::getList();
    // allocate storage for the wrappers
    net->myTLLogicWrappers.reserve(tlls.size());
    // go through the logics
    for(LogicVector::iterator i=tlls.begin(); i!=tlls.end(); i++) {
        // get the logic
        MSTrafficLightLogic *tll = (*i);
        // build the wrapper
        GUITrafficLightLogicWrapper *tllw =
            new GUITrafficLightLogicWrapper(gIDStorage, *tll);
        // get the links
        const MSTrafficLightLogic::LinkVectorVector &links =
            tll->getLinks();
        // build the association link->wrapper
        MSTrafficLightLogic::LinkVectorVector::const_iterator j;
        for(j=links.begin(); j!=links.end(); j++) {
            MSTrafficLightLogic::LinkVector::const_iterator j2;
            for(j2=(*j).begin(); j2!=(*j).end(); j2++) {
                net->myLinks2Logic[*j2] = tllw;
            }
        }
        // save the wrapper
    }
}


Position2D
GUINet::getJunctionPosition(const std::string &name) const
{
    MSJunction *junction = MSJunction::dictionary(name);
    return Position2D(junction->getPosition());
}


Position2D
GUINet::getVehiclePosition(const std::string &name, bool useCenter) const
{
    MSVehicle *vehicle = MSVehicle::dictionary(name);
    if(vehicle==0) {
        throw GUIExcp_VehicleIsInvisible();
    }
    const GUIEdge * const edge =
        static_cast<const GUIEdge * const>(vehicle->getEdge());
    if(edge==0) {
        throw GUIExcp_VehicleIsInvisible();
    }
    double pos = vehicle->pos();
    if(useCenter) {
        pos -= (vehicle->length() / 2.0);
    }
    return edge->getLanePosition(vehicle->getLane(), pos);
}

/*
Position2D
GUINet::getDetectorPosition(const std::string &name) const
{
    std::map<std::string, GUIDetectorWrapper*>::const_iterator i=
        myDetectorDict.find(name);
    assert(i!=myDetectorDict.end());
    GUIDetectorWrapper *tmp = (*i).second;
    return (*i).second->getPosition();
}
*/
/*
Position2D
GUINet::getEmitterPosition(const std::string &name) const
{
    std::map<std::string, GUIEmitterWrapper*>::const_iterator i=
        myEmitterDict.find(name);
    assert(i!=myEmitterDict.end());
    return (*i).second->getPosition();
}
*/
/*
Position2D
GUINet::getTriggerPosition(const std::string &name) const
{
    GUILaneSpeedTrigger *trigger =
        static_cast<GUILaneSpeedTrigger*>
            (NamedObjectContSingleton<MSTrigger*>::getInstance().get(name));
    assert(trigger!=0);
    return trigger->getPosition();
}
*/

bool
GUINet::vehicleExists(const std::string &name) const
{
    return MSVehicle::dictionary(name)!=0;
}


Boundery
GUINet::getEdgeBoundery(const std::string &name) const
{
    GUIEdge *edge = static_cast<GUIEdge*>(MSEdge::dictionary(name));
    return edge->getBoundery();
}

/*
size_t
GUINet::getDetectorWrapperNo() const
{
    // !!! maybe this should return all the values for lanes, junction, detectors etc.
    return myAdditionalWrapper.size();
}
*/

GUINetWrapper *
GUINet::getWrapper() const
{
    return myWrapper;
}


unsigned int
GUINet::getLinkTLID(MSLink *link) const
{
    std::map<MSLink*, GUITrafficLightLogicWrapper*>::const_iterator i =
        myLinks2Logic.find(link);
    assert(i!=myLinks2Logic.end());
    return (*i).second->getGlID();
}


void
GUINet::guiSimulationStep()
{
    MSUpdateEachTimestepContainer<MSUpdateEachTimestep<GUILaneStateReporter> >::getInstance()->updateAll();
    MSUpdateEachTimestepContainer<MSUpdateEachTimestep<GLObjectValuePassConnector<double> > >::getInstance()->updateAll();
    MSUpdateEachTimestepContainer<MSUpdateEachTimestep<GLObjectValuePassConnector<CompletePhaseDef> > >::getInstance()->updateAll();
}


std::vector<size_t>
GUINet::getJunctionIDs() const
{
    std::vector<size_t> ret;
    for(std::vector<GUIJunctionWrapper*>::const_iterator i=myJunctionWrapper.begin(); i!=myJunctionWrapper.end(); i++) {
        ret.push_back((*i)->getGlID());
    }
    return ret;
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:


