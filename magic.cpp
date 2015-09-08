#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <QtGlobal>

#include "magic.h"

class Matcher {
  public:
    virtual bool matches(std::ifstream &) = 0;

  protected:
    bool read(std::ifstream &stream, off_t offset, void *buf, size_t n)
    {
      return !stream.seekg(offset, std::ios_base::beg).fail() && !stream.read(reinterpret_cast<char *>(buf), n).fail();
    }
};

class Magic : public Matcher
{
  public:
    Magic(size_t offset, const char *magic) : offset(offset), magic(magic), len(strlen(magic)) { }
    Magic(size_t offset, const char *magic, size_t len) : offset(offset), magic(magic), len(len) { }

    bool matches(std::ifstream &stream)
    {
      std::vector<char> buf(len);

      return read(stream, offset, buf.data(), buf.size()) && std::memcmp(buf.data(), magic, buf.size()) == 0;
    }

  private:
    const size_t offset;
    const char *magic;
    const size_t len;
};

/* DMO decrypt code based on Adplug. */
class DMOMatcher : public Matcher
{
  public:
    bool matches(std::ifstream &stream)
    {
      unsigned char header[12];

      return read(stream, 0, header, sizeof header) && decrypt(header);
    }

  private:
    quint32 bseed;

    inline quint16 le16(const unsigned char *p) { return ((quint16)    ( p[1]) <<  8) |     ( p[0]); }
    inline quint32 le32(const unsigned char *p) { return ((quint32)le16(&p[2]) << 16) | le16(&p[0]); }

    quint16 LOWORD(quint32 l) { return l & 0xffff; }
    quint16 HIWORD(quint32 l) { return l >> 16; }
    quint8  LOBYTE(quint16 w) { return w & 0xff; }
    quint8  HIBYTE(quint16 w) { return w >> 8; }

    int brand(quint16 range)
    {
      quint16 ax, bx, cx, dx;

      ax = LOWORD(bseed);
      bx = HIWORD(bseed);
      cx = ax;
      ax = LOWORD(cx * 0x8405);
      dx = HIWORD(cx * 0x8405);
      cx <<= 3;
      cx = (((HIBYTE(cx) + LOBYTE(cx)) & 0xff) << 8) + LOBYTE(cx);
      dx += cx;
      dx += bx;
      bx <<= 2;
      dx += bx;
      dx = (((HIBYTE(dx) + LOBYTE(bx)) & 0xff) << 8) + LOBYTE(dx);
      bx <<= 5;
      dx = (((HIBYTE(dx) + LOBYTE(bx)) & 0xff) << 8) + LOBYTE(dx);
      ax += 1;
      if(ax == 0) dx += 1;

      bseed = dx;
      bseed <<= 16;
      bseed += ax;

      return HIWORD(HIWORD(LOWORD(bseed) * range) + HIWORD(bseed) * range);
    }

    bool decrypt(unsigned char *header)
    {
      quint32 seed = 0;
      unsigned char zeros[12] = { 0 };

      /* The DMO decrypt algorithm successfully “decrypts” all nulls,
       * which results in a number of false positives.  No .dmo file
       * I've seen starts with all nulls, so this should be OK.
       */
      if(std::memcmp(header, zeros, sizeof zeros) == 0) return false;

      bseed = le32(&header[0]);

      for(int i = 0; i < le16(&header[4]) + 1; i++) seed += brand(0xffff);

      bseed = seed ^ le32(&header[6]);

      return le16(&header[10]) == brand(0xffff);
    }
};

class OriginMatcher : public Matcher
{
  /* I've found few false-positives with this... */
  bool matches(std::ifstream &stream)
  {
    unsigned char header[6];

    if(!read(stream, 0, header, sizeof header)) return false;

    /* TrueType files match Origin magic, so try to detect them here.
     * All .m files I've tested pass this.
     */
    if(std::memcmp(header, "\0\1\0\0\0", 5) == 0)
    {
      return false;
    }

    return header[2] == 0 && header[3] == 0 && header[4] + ((header[5] & 0x1) << 8) == 0x100;
  }
};

bool adplug_supports(std::string filename)
{
  std::ifstream stream(filename.c_str(), std::ios::binary);

  if(!stream)
  {
    return false;
  }

  std::vector<std::shared_ptr<Matcher>> matchers = {
    /* AdLib Tracker 2 (.a2m) */
    std::shared_ptr<Matcher>(new Magic(0, "_A2module_")),

    /* AMUSIC Adlib Tracker (.amd) */
    std::shared_ptr<Matcher>(new Magic(1062, "<o\xefQU\xeeRoR")),

    /* Bob's Adlib Music Format (.bam) */
    std::shared_ptr<Matcher>(new Magic(0, "CBMF")),

    /* BoomTracker 4.0 (.cff) */
    std::shared_ptr<Matcher>(new Magic(0, "<CUD-FM-File>")),

    /* Creative Music File Format (.cmf) */
    std::shared_ptr<Matcher>(new Magic(0, "CTMF")),

    /* EdLib (.d00) */
    std::shared_ptr<Matcher>(new Magic(0, "JCH\x26\x02\x66")),

    /* Digital-FM (.dfm) */
    std::shared_ptr<Matcher>(new Magic(0, "DFM\x1a")),

    /* DOSBox Raw OPL (.dro) */
    std::shared_ptr<Matcher>(new Magic(0, "DBRAWOPL")),

    /* DeFy Adlib Tracker (.dtm) */
    std::shared_ptr<Matcher>(new Magic(0, "DeFy DTM ")),

    /* LucasArts AdLib Audio (.laa) */
    std::shared_ptr<Matcher>(new Magic(0, "ADL")),

    /* Mlat Adlib Tracker (.mad) */
    std::shared_ptr<Matcher>(new Magic(0, "MAD+")),

    /* MKJamz (.mkj) */
    std::shared_ptr<Matcher>(new Magic(0, "MKJamz")),

    /* AdLib MSCplay (.msc) */
    std::shared_ptr<Matcher>(new Magic(0, "Ceres \x13 MSCplay ")),

    /* MPU-401 Trakker (.mtk) */
    std::shared_ptr<Matcher>(new Magic(0, "mpu401tr\x92kk\xeer@data")),

    /* Reality ADlib Tracker (.rad) */
    std::shared_ptr<Matcher>(new Magic(0, "RAD by REALiTY!!")),

    /* RdosPlay RAW file format (.raw) */
    std::shared_ptr<Matcher>(new Magic(0, "RAWADATA")),

    /* Softstar RIX OPL (.rix) */
    std::shared_ptr<Matcher>(new Magic(0, "\xaa\x55")),

    /* AdLib Visual Composer (.rol) */
    std::shared_ptr<Matcher>(new Magic(0, "\0\0\4\0", 4)),

    /* Surprise! Adlib Tracker 1 & 2 (.sat, .sa2) */
    std::shared_ptr<Matcher>(new Magic(0, "SAdT")),

    /* Sierra's AdLib Audio (.sci) */
    std::shared_ptr<Matcher>(new Magic(0, "\x84\x00", 2)),

    /* SNGPlay (.sng) */
    std::shared_ptr<Matcher>(new Magic(0, "ObsM")),

    /* Faust Music Creator (.sng) */
    std::shared_ptr<Matcher>(new Magic(0, "FMC!")),

    /* eXotic ADlib Format (.xad) */
    std::shared_ptr<Matcher>(new Magic(0, "XAD!")),

    /* XMS-Tracker (.xms) */
    std::shared_ptr<Matcher>(new Magic(1062, "MaDoKaN96")),

    /* eXtra Simple Music (.xsm) */
    std::shared_ptr<Matcher>(new Magic(0, "ofTAZ!")),

    /* Twin TrackPlayer by TwinTeam (.dmo) */
    std::shared_ptr<Matcher>(new DMOMatcher()),

    /* Origin AdLib Music Format (.m) */
    std::shared_ptr<Matcher>(new OriginMatcher()),
  };

  return std::any_of(
      matchers.begin(), matchers.end(),
      [&stream](const std::shared_ptr<Matcher> &matcher){ return matcher->matches(stream); }
  );
}
