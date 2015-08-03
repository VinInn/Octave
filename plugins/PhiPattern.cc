#include "Producer.h"
#include "WhiteBoard.h"
#include "Config_PhiPattern.h"

#include "Geometry.h"

#include "TSimHits.h"

#include "cephes.h"
#include <array>

/* makes pattern recognition in phi using a trivial histogram method

 */
class PhiPattern : public Producer, private  Config_PhiPattern {
public:
  typedef Config_PhiPattern Configuration;
  explicit  PhiPattern(Configuration const& config) : Configuration(config) {}

  int produce(WhiteBoard & event);


};

#include<cassert>
int PhiPattern::produce(WhiteBoard & event) {
  using namespace GeometryConstants;
  // we know is 1 (sic)
  auto const & hits = event.get<TSimHits>(1);

  // get the workspace
  auto workspace = event.workspace();

  int hist[m_nbin];
  std::fill_n(hist,m_nbin,0);
  //const size_t nbin = m_nbin;
  //std::array<int,nbin> hist;

  // can be uttelry optimized

  /*
  auto kernel = [&hits, &hist](int i) {
    int oct = hits.m_id[i]&7; // octant
    int il = hits.m_id[i] >>3; // layer
    assert(oct<8);
    assert(il<12);
    float xp = xpitch[il];
    float r= GeometryConstants::radii[il];
    float phi0 = GeometryConstants::pio4*oct;
    float  d= (0.5f+hits.m_x[i])*xp;
    assert(d>=0);
    assert(d<sides[il]);
    assert( r > d*sinPIo8);
    float phi = phi0 + cephes::atan2f(d*cosPIo8, r - d*sinPIo8);
    assert(phi>=0);
    assert(phi<GeometryConstants::pi2f);
    ++hist[int(m_nbin*phi/GeometryConstants::pi2f)];
  };

  int loops = hits.m_size;
  // #pragma ivdep
  for (int i=0; i!=loops; ++i) {
    kernel(i);
  }
  */
  
  auto kernel0 = [&hits, this](int i, float & xp, float & r, float & phi0) {
    int oct = hits.m_id[i]&7; // octant
    int il = hits.m_id[i] >>3; // layer
    assert(oct<8);
    assert(il<12);
    xp = xpitch[il];
    r= GeometryConstants::radii[il];
    phi0 = GeometryConstants::pio4*oct;
 
  };

  auto kernel1 = [&hits,this](int i, float xp, float  r, float phi0) {
    float  d= (0.5f+hits.m_x[i])*xp;
    float phi = phi0 + cephes::atan2f(d*cosPIo8, r - d*sinPIo8);
    return int(m_nbin*phi/GeometryConstants::pi2f);
  };
  
  constexpr int stride = 8;
  int loops = hits.m_size/stride;
  int tail =  hits.m_size - stride*loops;
  float xp[stride];
  float r[stride];
  float phi0[stride];
  int bin[stride];
  for (int l=0; l!=loops; ++l) {
    int is=stride*l;
    for (int i=0; i!=stride; ++i) 
      kernel0(is+i,xp[i],r[i],phi0[i]);
    for (int i=0; i!=stride; ++i) 
      bin[i] = kernel1(is+i,xp[i],r[i],phi0[i]);
    for (int i=0; i!=stride; ++i) ++hist[bin[i]];
  }
  int is=stride*loops;
  for (int i=0; i!=tail; ++i) {
    kernel0(is+i,xp[i],r[i],phi0[i]);
    ++hist[kernel1(is+i,xp[i],r[i],phi0[i])];
  }


#ifndef NDEBUG
  {
    int i=0;
    while(i <m_nbin) {
      for (int j=0; j!=20; ++j)
	printf("%2d,",hist[i++]);
      printf("\n");
    }
    printf("\n");

  }
#endif

  // sliding window 
    // this is huge
    float phicl[m_nbin/2];
    const int thr0=4;
    const int thr1=9;
    int nc=0;
  {
    int i=0;
    while( i<m_nbin) {
      if (hist[i]<thr0) { ++i; continue;}
      // new cluster candidate
      int sum=0; int j=i-1; float ave=0;
      while(j>=0 &&  hist[j]>0) { sum+=hist[j]; ave+=hist[j]*j; hist[j]=0; --j;}
      while(i<m_nbin && hist[i]>0) { sum+=hist[i]; ave+=hist[i]*i; hist[i]=0; ++i;} // add valley detection
      if (sum>thr1) {
	phicl[nc]= ave/sum;
	nc++;
      }
    }
  }

#ifndef NDEBUG
  printf("%d: ",nc);
  for (int i=0;i!=nc;++i) 
    printf("%g,",phicl[i]);
  printf("\n");
  printf("\n");
#endif

  std::vector<float> final(phicl, phicl+nc);
  // put it in the event
  event.put(std::move(final));

  return 0;
}
  
PRODUCER_PLUGIN(PhiPattern);
