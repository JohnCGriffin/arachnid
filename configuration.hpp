
#ifndef _CONFIGURATION_HPP_1365261201_
#define _CONFIGURATION_HPP_1365261201_

#include <string>

namespace arachnid {

  void read_configuration ( std::string config_file );

}

/*-------- sample argv[1] configuration file ------------------
arachnid.johngriffin.biz 4567 /benchmark/ BenchmarkHandler {
}

arachnid.johngriffin.biz 4567 /scgi/ SCGIHandler {
	port 1998
	host localhost
}

arachnid.johngriffin.biz 4567 /source/ CSourceHandler {
	documentRoot /home/arachnid/
	directoryListing true
}

arachnid.johngriffin.biz 4567 /css/ FileHandler {
	documentRoot /home/arachnid/
	directoryListing false
}

arachnid.johngriffin.biz 4567 /js/ FileHandler {
	documentRoot /home/arachnid/
	directoryListing false
}

arachnid.johngriffin.biz 4567 /images/ FileHandler {
	documentRoot /home/arachnid/
	directoryListing false
}

arachnid.johngriffin.biz 4567 / FileHandler {
	documentRoot /home/arachnid/document-root
	directoryListing false
}

------------------------------------------------------------------ */


#endif

