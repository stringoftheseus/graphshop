HEADERS += \
	graphcore/_graphcore_h.h \
	graphcore/_graphcore_c.h \
    graphcore/graph.h \
    graphcore/edge.h \
    graphcore/arc.h \
    graphcore/vertex.h \
    graphio/graphiotext.h \
	qScriptRegisterQObjectMetaType.h \
	graphshopwindow.h \
	windows/draw/graphdraw.h \
	windows/draw/edgedraw.h \
	windows/draw/drawwindow.h \
	windows/draw/drawtables.h \
	windows/draw/vertexdraw.h \
	windows/script/scriptwindow.h \
	windows/script/codewindow.h \
	windows/draw/arcdraw.h \
	windows/adjmatrix/adjmatrixwindow.h \
	windows/graphwindow.h \
	graphaction.h \
	windows/draw/layout/drawlayout.h \
	windows/draw/layout/randomlayout.h \
	windows/draw/layout/circlelayout.h \
	windows/draw/layout/gravitylayout.h \
    graphshopapp.h \
    graphmenu.h \
    graphpack.h \
    windows/_panels_h.h \
	windows/_panels_c.h \
    graphcore/operations/graphoperation.h \
    graphcore/operations/dominationgraph.h \
    graphcore/operations/conversegraph.h \
    graphcore/operations/complementgraph.h \
    graphcore/operations/competitiongraph.h \
    graphcore/operations/_graphoperations_h.h \
    graphcore/operations/_graphoperations_c.h \
    graphcore/operations/underlyinggraph.h \
    graphcore/representations/_graphrepresentations_c.h \
	graphcore/representations/_graphrepresentations_h.h \
    graphcore/representations/graphrepresentation.h \
    graphcore/representations/tournament.h \
	windows/tournament/tournamentwindow.h \
	windows/tournament/tournamentvertexdraw.h \
	windows/tournament/tournamentdraw.h \
	windows/tournament/tournamentarcdraw.h \
    graphcore/representations/intervalgraph.h \
    graphcore/representations/interval.h \
    windows/intervalgraph/intervalgraphwindow.h \
    windows/intervalgraph/intervalgraphdraw.h \
    windows/intervalgraph/intervaldraw.h

SOURCES += graphcore/vertex.cpp \
	graphcore/graph.cpp \
    graphcore/edge.cpp \
    graphcore/arc.cpp \
    graphio/graphiotext.cpp \
	main.cpp \
	graphshopwindow.cpp \
	windows/draw/vertexdraw.cpp \
	windows/draw/graphdraw.cpp \
	windows/draw/edgedraw.cpp \
	windows/draw/drawwindow.cpp \
	windows/script/scriptwindow.cpp \
	windows/script/codewindow.cpp \
	windows/draw/arcdraw.cpp \
	windows/adjmatrix/adjmatrixwindow.cpp \
	windows/graphwindow.cpp \
	graphaction.cpp \
	windows/draw/layout/drawlayout.cpp \
	windows/draw/layout/randomlayout.cpp \
	windows/draw/layout/circlelayout.cpp \
	windows/draw/layout/gravitylayout.cpp \
    graphshopapp.cpp \
    graphmenu.cpp \
	graphpack.cpp \
    graphcore/operations/underlyinggraph.cpp \
    graphcore/operations/graphoperation.cpp \
    graphcore/operations/dominationgraph.cpp \
    graphcore/operations/conversegraph.cpp \
    graphcore/operations/complementgraph.cpp \
    graphcore/operations/competitiongraph.cpp \
    graphcore/representations/graphrepresentation.cpp \
    graphcore/representations/tournament.cpp \
	windows/tournament/tournamentwindow.cpp \
	windows/tournament/tournamentvertexdraw.cpp \
	windows/tournament/tournamentdraw.cpp \
	windows/tournament/tournamentarcdraw.cpp \
    graphcore/representations/intervalgraph.cpp \
    graphcore/representations/interval.cpp \
    windows/intervalgraph/intervalgraphwindow.cpp \
    windows/intervalgraph/intervalgraphdraw.cpp \
    windows/intervalgraph/intervaldraw.cpp

OTHER_FILES += dev/notes.txt

QT += script
