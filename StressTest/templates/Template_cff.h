#include "Registerer.h"
#include "Config_Template.h"
#include <cmath>

namespace {
  
  namespace details_Template_cff {
    
    static Config_Template c10;
    REGISTER(Template, Template_10, c10)
    
    static Config_CPSGenerator c100;
    REGISTER(Template, Template_100, c100)
    
    struct L1 {
      L1() {
	c10.aveN=10;
	c10.afloat=3.14.f;
	c10.adouble=sin(1.2);
	c100.aveN=100;
	c100.afloat=3.14.f;
	c100.adouble=cos(1.2);
      }
    };
    static L1 l1;
    
  }
  
}
