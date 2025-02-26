#ifndef ParticleFlowCandidate_PFCandidate_h
#define ParticleFlowCandidate_PFCandidate_h
/** \class reco::PFCandidate
 *
 * particle candidate from particle flow
 *
 */

#if !defined(__CINT__) && !defined(__MAKECINT__) && !defined(__REFLEX__)
#include <atomic>
#endif
#include <iosfwd>
#include <array>

#include "DataFormats/Math/interface/Point3D.h"

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateElectronExtraFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertexFwd.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidatePhotonExtraFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateEGammaExtraFwd.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
namespace reco {
  /**\class PFCandidate
     \brief Particle reconstructed by the particle flow algorithm.
          
     \author Colin Bernet
     \date   February 2007
  */

  class PFCandidate : public CompositeCandidate {
  public:
    /// particle types
    enum ParticleType {
      X = 0,     // undefined
      h,         // charged hadron
      e,         // electron
      mu,        // muon
      gamma,     // photon
      h0,        // neutral hadron
      h_HF,      // HF tower identified as a hadron
      egamma_HF  // HF tower identified as an EM particle
    };

    enum Flags {
      NORMAL = 0,
      E_PHI_SMODULES,
      E_ETA_0,
      E_ETA_MODULES,
      E_BARREL_ENDCAP,
      E_PRESHOWER_EDGE,
      E_PRESHOWER,
      E_ENDCAP_EDGE,
      H_ETA_0,
      H_BARREL_ENDCAP,
      H_ENDCAP_VFCAL,
      H_VFCAL_EDGE,
      T_TO_DISP,
      T_FROM_DISP,
      T_FROM_V0,
      T_FROM_GAMMACONV,
      GAMMA_TO_GAMMACONV
    };

    enum PFVertexType {
      kCandVertex = 0,
      kTrkVertex = 1,
      kComMuonVertex = 2,
      kSAMuonVertex = 3,
      kTrkMuonVertex = 4,
      kGSFVertex = 5,
      kTPFMSMuonVertex = 6,
      kPickyMuonVertex = 7,
      kDYTMuonVertex = 8
    };

    /// default constructor
    PFCandidate();

    /// constructor from a reference (keeps track of source relationship)
    PFCandidate(const PFCandidatePtr& sourcePtr);

    /*     PFCandidate( Charge q,  */
    /*                  const LorentzVector & p4,  */
    /*                  ParticleType particleId,  */
    /*                  reco::PFBlockRef blockRef ); */
    PFCandidate(Charge q, const LorentzVector& p4, ParticleType particleId);

    /// copy constructor
    PFCandidate(const PFCandidate&);

    /// destructor
    ~PFCandidate() override;

    PFCandidate& operator=(PFCandidate const&);

    /// return a clone
    PFCandidate* clone() const override;

    /*    /// set source ref */
    /*     void setSourceRef(const PFCandidateRef& ref) { sourceRef_ = ref; } */

    /*     size_type numberOfSourceCandidateRefs() const {return 1;} */

    /*     CandidateBaseRef sourceCandidateRef( size_type i ) const { */
    /*       return  CandidateBaseRef(sourceRef_); */
    /*     } */

    using reco::Candidate::setSourceCandidatePtr;
    void setSourceCandidatePtr(const PFCandidatePtr& ptr) { sourcePtr_ = ptr; }

    size_t numberOfSourceCandidatePtrs() const override { return 1; }

    CandidatePtr sourceCandidatePtr(size_type i) const override { return sourcePtr_; }

    /// returns the pdg id corresponding to the particle type.
    /// the particle type could be removed at some point to gain some space.
    /// low priority
    int translateTypeToPdgId(ParticleType type) const;
    ParticleType translatePdgIdToType(int pdgid) const;

    /// set Particle Type
    void setParticleType(ParticleType type);

    /// add an element to the current PFCandidate
    /*     void addElement( const reco::PFBlockElement* element ); */

    /// add element in block
    void addElementInBlock(const reco::PFBlockRef& blockref, unsigned elementIndex);

    /// set track reference
    void setTrackRef(const reco::TrackRef& ref);

    /// return a reference to the corresponding track, if charged.
    /// otherwise, return a null reference
    reco::TrackRef trackRef() const;

    /// return a pointer to the best track, if available.
    /// otherwise, return a null pointer
    const reco::Track* bestTrack() const override {
      if ((abs(pdgId()) == 11 || pdgId() == 22) && gsfTrackRef().isNonnull() && gsfTrackRef().isAvailable())
        return &(*gsfTrackRef());
      else if (trackRef().isNonnull() && trackRef().isAvailable())
        return &(*trackRef());
      else
        return nullptr;
    }
    /// uncertainty on dz
    float dzError() const override {
      const Track* tr = bestTrack();
      if (tr != nullptr)
        return tr->dzError();
      else
        return 0;
    }
    /// uncertainty on dxy
    float dxyError() const override {
      const Track* tr = bestTrack();
      if (tr != nullptr)
        return tr->dxyError();
      else
        return 0;
    }

    /// set gsftrack reference
    void setGsfTrackRef(const reco::GsfTrackRef& ref);

    /// return a reference to the corresponding GSF track, if an electron.
    /// otherwise, return a null reference
    reco::GsfTrackRef gsfTrackRef() const;

    /// set muon reference
    void setMuonRef(const reco::MuonRef& ref);

    /// return a reference to the corresponding muon, if a muon.
    /// otherwise, return a null reference
    reco::MuonRef muonRef() const;

    /// set displaced vertex reference
    void setDisplacedVertexRef(const reco::PFDisplacedVertexRef& ref, Flags flag);

    /// return a reference to the corresponding displaced vertex,
    /// otherwise, return a null reference
    reco::PFDisplacedVertexRef displacedVertexRef(Flags type) const;

    /// set ref to original reco conversion
    void setConversionRef(const reco::ConversionRef& ref);

    /// return a reference to the original conversion
    reco::ConversionRef conversionRef() const;

    /// set ref to original reco conversion
    void setV0Ref(const reco::VertexCompositeCandidateRef& ref);

    /// return a reference to the original conversion
    reco::VertexCompositeCandidateRef v0Ref() const;

    /// return a reference to the corresponding GsfElectron if any
    reco::GsfElectronRef gsfElectronRef() const;

    /// return a reference to the electron extra
    reco::PFCandidateElectronExtraRef electronExtraRef() const;

    /// set corrected Ecal energy
    void setEcalEnergy(float eeRaw, float eeCorr) {
      rawEcalEnergy_ = eeRaw;
      ecalERatio_ = std::abs(eeRaw) < 1.e-6 ? 1.0 : eeCorr / eeRaw;
    }

    /// return corrected Ecal energy
    double ecalEnergy() const { return ecalERatio_ * rawEcalEnergy_; }

    /// return corrected Ecal energy
    double rawEcalEnergy() const { return rawEcalEnergy_; }

    /// set corrected Hcal energy
    void setHcalEnergy(float ehRaw, float ehCorr) {
      rawHcalEnergy_ = ehRaw;
      hcalERatio_ = std::abs(ehRaw) < 1.e-6 ? 1.0 : ehCorr / ehRaw;
    }

    /// return corrected Hcal energy
    double hcalEnergy() const { return hcalERatio_ * rawHcalEnergy_; }

    /// return raw Hcal energy
    double rawHcalEnergy() const { return rawHcalEnergy_; }

    /// set corrected Hcal energy
    void setHoEnergy(float eoRaw, float eoCorr) {
      rawHoEnergy_ = eoRaw;
      hoERatio_ = std::abs(eoRaw) < 1.e-6 ? 1.0 : eoCorr / eoRaw;
    }

    /// return corrected Hcal energy
    double hoEnergy() const { return hoERatio_ * rawHoEnergy_; }

    /// return raw Hcal energy
    double rawHoEnergy() const { return rawHoEnergy_; }

    /// set GsfElectronRef
    void setGsfElectronRef(const reco::GsfElectronRef& ref);

    void setSuperClusterRef(const reco::SuperClusterRef& scRef);

    /// return a reference to the corresponding SuperCluster if any
    reco::SuperClusterRef superClusterRef() const;

    /// set ref to the corresponding reco::Photon if any
    void setPhotonRef(const reco::PhotonRef& phRef);

    /// return a reference to the corresponding Photon if any
    reco::PhotonRef photonRef() const;

    /// set the PF Photon Extra Ref
    void setPFPhotonExtraRef(const reco::PFCandidatePhotonExtraRef& ref);

    /// return a reference to the photon extra
    reco::PFCandidatePhotonExtraRef photonExtraRef() const;

    /// set the PF EGamma Extra Ref
    void setPFEGammaExtraRef(const reco::PFCandidateEGammaExtraRef& ref);

    /// return a reference to the EGamma extra
    reco::PFCandidateEGammaExtraRef egammaExtraRef() const;

    /// set corrected PS1 energy
    void setPs1Energy(float e1) { ps1Energy_ = e1; }

    /// return corrected PS1 energy
    double pS1Energy() const { return ps1Energy_; }

    /// set corrected PS2 energy
    void setPs2Energy(float e2) { ps2Energy_ = e2; }

    /// return corrected PS2 energy
    double pS2Energy() const { return ps2Energy_; }

    /// particle momentum *= rescaleFactor
    void rescaleMomentum(double rescaleFactor);

    /// set a given flag
    void setFlag(Flags theFlag, bool value);

    /// return a given flag
    bool flag(Flags theFlag) const;

    /// set uncertainty on momentum
    void setDeltaP(double dp) { deltaP_ = dp; }

    /// uncertainty on 3-momentum
    double deltaP() const { return deltaP_; }

    //  int pdgId() const { return translateTypeToPdgId( particleId_ ); }

    /// set mva for electron-pion discrimination.
    /// For charged particles, this variable is set
    ///   to 0 for particles that are not preided
    ///   to 1 otherwise
    /// For neutral particles, it is set to the default value

    void set_mva_Isolated(float mvaI) { mva_Isolated_ = mvaI; }
    // mva for isolated electrons
    float mva_Isolated() const { return mva_Isolated_; }

    void set_mva_e_pi(float mvaNI) { mva_e_pi_ = mvaNI; }
    /// mva for electron-pion discrimination
    float mva_e_pi() const { return mva_e_pi_; }

    /// set mva for electron-muon discrimination
    void set_mva_e_mu(float mva) { mva_e_mu_ = mva; }

    /// mva for electron-muon discrimination
    float mva_e_mu() const { return mva_e_mu_; }

    /// set mva for pi-muon discrimination
    void set_mva_pi_mu(float mva) { mva_pi_mu_ = mva; }

    /// mva for pi-muon discrimination
    float mva_pi_mu() const { return mva_pi_mu_; }

    /// set mva for gamma detection
    void set_mva_nothing_gamma(float mva) { mva_nothing_gamma_ = mva; }

    /// mva for gamma detection
    float mva_nothing_gamma() const { return mva_nothing_gamma_; }

    /// set mva for neutral hadron detection
    void set_mva_nothing_nh(float mva) { mva_nothing_nh_ = mva; }

    /// mva for neutral hadron detection
    float mva_nothing_nh() const { return mva_nothing_nh_; }

    /// set mva for neutral hadron - gamma discrimination
    void set_mva_gamma_nh(float mva) { mva_gamma_nh_ = mva; }

    // set DNN for electron PFID
    // mva for ele PFID DNN sigIsolated class
    float dnn_e_sigIsolated() const { return dnn_e_sigIsolated_; }
    void set_dnn_e_sigIsolated(float mva) { dnn_e_sigIsolated_ = mva; }

    // mva for ele PFID DNN sigNonIsolated class
    float dnn_e_sigNonIsolated() const { return dnn_e_sigNonIsolated_; }
    void set_dnn_e_sigNonIsolated(float mva) { dnn_e_sigNonIsolated_ = mva; }

    // mva for ele PFID DNN bkgNonIsolated class
    float dnn_e_bkgNonIsolated() const { return dnn_e_bkgNonIsolated_; }
    void set_dnn_e_bkgNonIsolated(float mva) { dnn_e_bkgNonIsolated_ = mva; }

    // mva for ele PFID DNN bkgTau class
    float dnn_e_bkgTau() const { return dnn_e_bkgTau_; }
    void set_dnn_e_bkgTau(float mva) { dnn_e_bkgTau_ = mva; }

    // mva for ele PFID DNN bkgPhoton class
    float dnn_e_bkgPhoton() const { return dnn_e_bkgPhoton_; }
    void set_dnn_e_bkgPhoton(float mva) { dnn_e_bkgPhoton_ = mva; }

    // set DNN for gamma PFID
    float dnn_gamma() const { return dnn_gamma_; }
    void set_dnn_gamma(float mva) { dnn_gamma_ = mva; }

    /// mva for neutral hadron - gamma discrimination
    float mva_gamma_nh() const { return mva_gamma_nh_; }

    /// set position at ECAL entrance
    void setPositionAtECALEntrance(const math::XYZPointF& pos) { positionAtECALEntrance_ = pos; }

    /// set the PF Electron Extra Ref
    void setPFElectronExtraRef(const reco::PFCandidateElectronExtraRef& ref);

    /// set the Best Muon Track Ref
    void setMuonTrackType(const reco::Muon::MuonTrackType& type) { muonTrackType_ = type; }

    /// get the Best Muon Track Ref

    const reco::Muon::MuonTrackType bestMuonTrackType() const { return muonTrackType_; }

    /// \return position at ECAL entrance
    const math::XYZPointF& positionAtECALEntrance() const { return positionAtECALEntrance_; }

    /// particle identification code
    /// \todo use Particle::pdgId_ and remove this data member
    virtual ParticleType particleId() const { return translatePdgIdToType(pdgId()); }

    /// return indices of elements used in the block
    /*     const std::vector<unsigned>& elementIndices() const {  */
    /*       return elementIndices_; */
    /*     } */
    /// return elements
    /*     const edm::OwnVector< reco::PFBlockElement >& elements() const  */
    /*       {return elements_;} */

    /// return elements in blocks
    typedef std::pair<reco::PFBlockRef, unsigned> ElementInBlock;
    typedef std::vector<ElementInBlock> ElementsInBlocks;

    typedef edm::RefVector<reco::PFBlockCollection> Blocks;
    typedef std::vector<unsigned> Elements;

    const ElementsInBlocks& elementsInBlocks() const;

    static constexpr float bigMva_ = -999.;

    friend std::ostream& operator<<(std::ostream& out, const PFCandidate& c);

    const Point& vertex() const override;
    double vx() const override { return vertex().x(); }
    double vy() const override { return vertex().y(); }
    double vz() const override { return vertex().z(); }

    /// do we have a valid time information
    bool isTimeValid() const { return timeError_ >= 0.f; }
    /// \return the timing
    float time() const { return time_; }
    /// \return the timing uncertainty
    float timeError() const { return timeError_; }
    /// \set the timing information
    void setTime(float time, float timeError = 0.f) {
      time_ = time;
      timeError_ = timeError;
    }

    /// fraction of hcal energy at a given depth  (depth = 1 .. 7)
    float hcalDepthEnergyFraction(unsigned int depth) const { return hcalDepthEnergyFractions_[depth - 1]; }
    /// fraction of hcal energy at a given depth (index 0..6 for depth 1..7)
    const std::array<float, 7>& hcalDepthEnergyFractions() const { return hcalDepthEnergyFractions_; }
    /// set the fraction of hcal energy as function of depth (index 0..6 for depth 1..7)
    void setHcalDepthEnergyFractions(const std::array<float, 7>& fracs) { hcalDepthEnergyFractions_ = fracs; }

  private:
    //function used before PR #31456, retained for backwards compatibility with old AOD where the vertex was not embedded
    const math::XYZPoint& vertexLegacy(PFCandidate::PFVertexType vertexType) const;

    /// Polymorphic overlap
    bool overlap(const Candidate&) const override;

    void setFlag(unsigned shift, unsigned flag, bool value);

    bool flag(unsigned shift, unsigned flag) const;

#if !defined(__CINT__) && !defined(__MAKECINT__) && !defined(__REFLEX__)
    mutable std::atomic<ElementsInBlocks*> elementsInBlocks_;
#else
    mutable ElementsInBlocks* elementsInBlocks_;
#endif
    Blocks blocksStorage_;
    Elements elementsStorage_;

    /// reference to the source PFCandidate, if any
    /*     PFCandidateRef sourceRef_; */
    PFCandidatePtr sourcePtr_;

    ///Reference to the best track if it is a muon
    ///pF is allowed to switch the default muon track
    reco::Muon::MuonTrackType muonTrackType_;

    /// corrected ECAL energy ratio (corrected/raw)
    float ecalERatio_;

    /// corrected HCAL energy ratio (corrected/raw)
    float hcalERatio_;

    /// corrected HO energy ratio (corrected/raw)
    float hoERatio_;

    /// raw ECAL energy
    float rawEcalEnergy_;

    /// raw HCAL energy
    float rawHcalEnergy_;

    /// raw HO energy
    float rawHoEnergy_;

    /// corrected PS1 energy
    float ps1Energy_;

    /// corrected PS2 energy
    float ps2Energy_;

    /// all flags, packed (ecal regional, hcal regional, tracking)
    unsigned flags_;

    /// uncertainty on 3-momentum
    float deltaP_;

    //legacy vertex type to read AOD created before PR #31456
    PFVertexType vertexType_;

    // mva for isolated electrons
    float mva_Isolated_;

    /// mva for electron-pion discrimination
    float mva_e_pi_;

    /// mva for electron-muon discrimination
    float mva_e_mu_;

    /// mva for pi-muon discrimination
    float mva_pi_mu_;

    /// mva for gamma detection
    float mva_nothing_gamma_;

    /// mva for neutral hadron detection
    float mva_nothing_nh_;

    /// mva for neutral hadron - gamma discrimination
    float mva_gamma_nh_;

    /// DNN for electron PFid: isolated signal
    float dnn_e_sigIsolated_;

    /// DNN for electron PFid: non-isolated signal
    float dnn_e_sigNonIsolated_;

    /// DNN for electron PFid: non-isolated bkg
    float dnn_e_bkgNonIsolated_;

    /// DNN for electron PFid: tau bkg
    float dnn_e_bkgTau_;

    /// DNN for electron PFid: photon bkg
    float dnn_e_bkgPhoton_;

    // DNN for gamma PFid
    float dnn_gamma_;

    /// position at ECAL entrance, from the PFRecTrack
    math::XYZPointF positionAtECALEntrance_;

    //more efficiently stored refs
    void storeRefInfo(unsigned int iMask,
                      unsigned int iBit,
                      bool iIsValid,
                      const edm::RefCore& iCore,
                      size_t iKey,
                      const edm::EDProductGetter*);
    bool getRefInfo(
        unsigned int iMask, unsigned int iBit, edm::ProductID& oProdID, size_t& oIndex, size_t& aIndex) const;

    const edm::EDProductGetter* getter_;  //transient
    unsigned short storedRefsBitPattern_;
    std::vector<uint64_t> refsInfo_;
    std::vector<const void*> refsCollectionCache_;

    /// timing information (valid if timeError_ >= 0)
    float time_;
    /// timing information uncertainty (<0 if timing not available)
    float timeError_;

    std::array<float, 7> hcalDepthEnergyFractions_;
  };

  /// particle ID component tag
  struct PFParticleIdTag {};

  /// get default PFBlockRef component
  /// as: pfcand->get<PFBlockRef>();
  /*   GET_DEFAULT_CANDIDATE_COMPONENT( PFCandidate, PFBlockRef, block ); */

  /// get int component
  /// as: pfcand->get<int, PFParticleIdTag>();
  GET_CANDIDATE_COMPONENT(PFCandidate, PFCandidate::ParticleType, particleId, PFParticleIdTag);

  std::ostream& operator<<(std::ostream& out, const PFCandidate& c);

}  // namespace reco

#endif
