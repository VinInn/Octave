#include "Producer.h"
#include "WhiteBoard.h"
#include "Config_Template.h"

#include<random>
#include<iostream>

#include "sfmt.h"

#include "Particles.h"
#include "cephes.h"

class Template : public Producer {
public:
  typedef Config_Template Configuration;

  explicit Template(Configuration const & config) : n(config.aveN)  {}

  int produce(WhiteBoard & event) { return 0;}

  int n;
}


PRODUCER_PLUGIN(Template);
