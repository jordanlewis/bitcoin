// Copyright (c) 2009-2010 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

class COutPoint;
class CInPoint;
class CDiskTxPos;
class CCoinBase;
class CTxIn;
class CTxOut;
class CTransaction;
class CBlock;
class CBlockIndex;
class CWalletTx;
class CKeyItem;

static const unsigned int MAX_BLOCK_SIZE = 1000000;
static const unsigned int MAX_BLOCK_SIZE_GEN = MAX_BLOCK_SIZE/2;
static const int MAX_BLOCK_SIGOPS = MAX_BLOCK_SIZE/50;
static const int64 COIN = 100000000;
static const int64 CENT = 1000000;
static const int64 MAX_MONEY = 21000000 * COIN;
inline bool MoneyRange(int64 nValue) { return (nValue >= 0 && nValue <= MAX_MONEY); }
static const int COINBASE_MATURITY = 100;
#ifdef USE_UPNP
static const int fHaveUPnP = true;
#else
static const int fHaveUPnP = false;
#endif






extern CCriticalSection cs_main;
extern map<uint256, CBlockIndex*> mapBlockIndex;
extern uint256 hashGenesisBlock;
extern CBigNum bnProofOfWorkLimit;
extern CBlockIndex* pindexGenesisBlock;
extern int nBestHeight;
extern CBigNum bnBestChainWork;
extern CBigNum bnBestInvalidWork;
extern uint256 hashBestChain;
extern CBlockIndex* pindexBest;
extern unsigned int nTransactionsUpdated;
extern map<uint256, int> mapRequestCount;
extern CCriticalSection cs_mapRequestCount;
extern map<string, string> mapAddressBook;
extern CCriticalSection cs_mapAddressBook;
extern vector<unsigned char> vchDefaultKey;
extern double dHashesPerSec;
extern int64 nHPSTimerStart;

// Settings
extern int fGenerateBitcoins;
extern int64 nTransactionFee;
extern CAddress addrIncoming;
extern int fLimitProcessors;
extern int nLimitProcessors;
extern int fMinimizeToTray;
extern int fMinimizeOnClose;
extern int fUseUPnP;







bool CheckDiskSpace(uint64 nAdditionalBytes=0);
FILE* OpenBlockFile(unsigned int nFile, unsigned int nBlockPos, const char* pszMode="rb");
FILE* AppendBlockFile(unsigned int& nFileRet);
bool AddKey(const CKey& key);
vector<unsigned char> GenerateNewKey();
bool AddToWallet(const CWalletTx& wtxIn);
void WalletUpdateSpent(const COutPoint& prevout);
int ScanForWalletTransactions(CBlockIndex* pindexStart);
void ReacceptWalletTransactions();
bool LoadBlockIndex(bool fAllowNew=true);
void PrintBlockTree();
bool ProcessMessages(CNode* pfrom);
bool ProcessMessage(CNode* pfrom, string strCommand, CDataStream& vRecv);
bool SendMessages(CNode* pto, bool fSendTrickle);
int64 GetBalance();
bool CreateTransaction(const vector<pair<CScript, int64> >& vecSend, CWalletTx& wtxNew, CReserveKey& reservekey, int64& nFeeRet);
bool CreateTransaction(CScript scriptPubKey, int64 nValue, CWalletTx& wtxNew, CReserveKey& reservekey, int64& nFeeRet);
bool CommitTransaction(CWalletTx& wtxNew, CReserveKey& reservekey);
bool BroadcastTransaction(CWalletTx& wtxNew);
string SendMoney(CScript scriptPubKey, int64 nValue, CWalletTx& wtxNew, bool fAskFee=false);
string SendMoneyToBitcoinAddress(string strAddress, int64 nValue, CWalletTx& wtxNew, bool fAskFee=false);
void GenerateBitcoins(bool fGenerate);
void ThreadBitcoinMiner(void* parg);
CBlock* CreateNewBlock(CReserveKey& reservekey);
void IncrementExtraNonce(CBlock* pblock, CBlockIndex* pindexPrev, unsigned int& nExtraNonce, int64& nPrevTime);
void FormatHashBuffers(CBlock* pblock, char* pmidstate, char* pdata, char* phash1);
bool CheckWork(CBlock* pblock, CReserveKey& reservekey);
void BitcoinMiner();
bool CheckProofOfWork(uint256 hash, unsigned int nBits);
bool IsInitialBlockDownload();
string GetWarnings(string strFor);












class CDiskTxPos
{
public:
    unsigned int nFile;
    unsigned int nBlockPos;
    unsigned int nTxPos;

    CDiskTxPos();
    CDiskTxPos(unsigned int nFileIn, unsigned int nBlockPosIn, unsigned int nTxPosIn);

    IMPLEMENT_SERIALIZE( READWRITE(FLATDATA(*this)); )

    void SetNull();
    bool IsNull() const;

    friend bool operator==(const CDiskTxPos& a, const CDiskTxPos& b)
    {
        return (a.nFile     == b.nFile &&
                a.nBlockPos == b.nBlockPos &&
                a.nTxPos    == b.nTxPos);
    }

    friend bool operator!=(const CDiskTxPos& a, const CDiskTxPos& b)
    {
        return !(a == b);
    }


    string ToString() const;

    void print() const;
};




class CInPoint
{
public:
    CTransaction* ptx;
    unsigned int n;

    CInPoint();
    CInPoint(CTransaction* ptxIn, unsigned int nIn);
    void SetNull();
    bool IsNull() const;
};




class COutPoint
{
public:
    uint256 hash;
    unsigned int n;

    COutPoint();
    COutPoint(uint256 hashIn, unsigned int nIn);
    IMPLEMENT_SERIALIZE( READWRITE(FLATDATA(*this)); )
    void SetNull();
    bool IsNull() const;

    friend bool operator<(const COutPoint& a, const COutPoint& b)
    {
        return (a.hash < b.hash || (a.hash == b.hash && a.n < b.n));
    }

    friend bool operator==(const COutPoint& a, const COutPoint& b)
    {
        return (a.hash == b.hash && a.n == b.n);
    }

    friend bool operator!=(const COutPoint& a, const COutPoint& b)
    {
        return !(a == b);
    }


    string ToString() const;
    void print() const;
};




//
// An input of a transaction.  It contains the location of the previous
// transaction's output that it claims and a signature that matches the
// output's public key.
//
class CTxIn
{
public:
    COutPoint prevout;
    CScript scriptSig;
    unsigned int nSequence;

    CTxIn();
    explicit CTxIn(COutPoint prevoutIn, CScript scriptSigIn=CScript(), unsigned int nSequenceIn=UINT_MAX);

    CTxIn(uint256 hashPrevTx, unsigned int nOut, CScript scriptSigIn=CScript(), unsigned int nSequenceIn=UINT_MAX);

    IMPLEMENT_SERIALIZE
    (
        READWRITE(prevout);
        READWRITE(scriptSig);
        READWRITE(nSequence);
    )

    bool IsFinal() const;
    friend bool operator==(const CTxIn& a, const CTxIn& b)
    {
        return (a.prevout   == b.prevout &&
                a.scriptSig == b.scriptSig &&
                a.nSequence == b.nSequence);
    }

    friend bool operator!=(const CTxIn& a, const CTxIn& b)
    {
        return !(a == b);
    }

    string ToString() const;

    void print() const;

    bool IsMine() const;
    int64 GetDebit() const;
};




//
// An output of a transaction.  It contains the public key that the next input
// must be able to sign with to claim it.
//
class CTxOut
{
public:
    int64 nValue;
    CScript scriptPubKey;

    CTxOut();

    CTxOut(int64 nValueIn, CScript scriptPubKeyIn);

    IMPLEMENT_SERIALIZE
    (
        READWRITE(nValue);
        READWRITE(scriptPubKey);
    )

    void SetNull();
    bool IsNull();

    uint256 GetHash() const;

    bool IsMine() const;

    int64 GetCredit() const;

    bool IsChange() const;
    int64 GetChange() const;

    friend bool operator==(const CTxOut& a, const CTxOut& b)
    {
        return (a.nValue       == b.nValue &&
                a.scriptPubKey == b.scriptPubKey);
    }

    friend bool operator!=(const CTxOut& a, const CTxOut& b)
    {
        return !(a == b);
    }

    string ToString() const;
    void print() const;
};




//
// The basic transaction that is broadcasted on the network and contained in
// blocks.  A transaction can contain multiple inputs and outputs.
//
class CTransaction
{
public:
    int nVersion;
    vector<CTxIn> vin;
    vector<CTxOut> vout;
    unsigned int nLockTime;


    CTransaction();

    IMPLEMENT_SERIALIZE
    (
        READWRITE(this->nVersion);
        nVersion = this->nVersion;
        READWRITE(vin);
        READWRITE(vout);
        READWRITE(nLockTime);
    )

    void SetNull();
    bool IsNull() const;

    uint256 GetHash() const;

    bool IsFinal(int nBlockHeight=0, int64 nBlockTime=0) const;
    bool IsNewerThan(const CTransaction& old) const;
    bool IsCoinBase() const;

    int GetSigOpCount() const;

    bool IsStandard() const;
    bool IsMine() const;
    bool IsFromMe() const;

    int64 GetDebit() const;
    int64 GetCredit() const;
    int64 GetChange() const;
    int64 GetValueOut() const;

    static bool AllowFree(double dPriority);

    int64 GetMinFee(unsigned int nBlockSize=1, bool fAllowFree=true) const;


    bool ReadFromDisk(CDiskTxPos pos, FILE** pfileRet=NULL);

    friend bool operator==(const CTransaction& a, const CTransaction& b)
    {
        return (a.nVersion  == b.nVersion &&
                a.vin       == b.vin &&
                a.vout      == b.vout &&
                a.nLockTime == b.nLockTime);
    }

    friend bool operator!=(const CTransaction& a, const CTransaction& b)
    {
        return !(a == b);
    }


    string ToString() const;
    void print() const;


    bool ReadFromDisk(CTxDB& txdb, COutPoint prevout, CTxIndex& txindexRet);
    bool ReadFromDisk(CTxDB& txdb, COutPoint prevout);
    bool ReadFromDisk(COutPoint prevout);
    bool DisconnectInputs(CTxDB& txdb);
    bool ConnectInputs(CTxDB& txdb, map<uint256, CTxIndex>& mapTestPool, CDiskTxPos posThisTx,
                       CBlockIndex* pindexBlock, int64& nFees, bool fBlock, bool fMiner, int64 nMinFee=0);
    bool ClientConnectInputs();
    bool CheckTransaction() const;
    bool AcceptToMemoryPool(CTxDB& txdb, bool fCheckInputs=true, bool* pfMissingInputs=NULL);
    bool AcceptToMemoryPool(bool fCheckInputs=true, bool* pfMissingInputs=NULL);
protected:
    bool AddToMemoryPoolUnchecked();
public:
    bool RemoveFromMemoryPool();
};





//
// A transaction with a merkle branch linking it to the block chain
//
class CMerkleTx : public CTransaction
{
public:
    uint256 hashBlock;
    vector<uint256> vMerkleBranch;
    int nIndex;

    // memory only
    mutable char fMerkleVerified;


    CMerkleTx();
    CMerkleTx(const CTransaction& txIn);

    void Init();

    IMPLEMENT_SERIALIZE
    (
        nSerSize += SerReadWrite(s, *(CTransaction*)this, nType, nVersion, ser_action);
        nVersion = this->nVersion;
        READWRITE(hashBlock);
        READWRITE(vMerkleBranch);
        READWRITE(nIndex);
    )


    int SetMerkleBranch(const CBlock* pblock=NULL);
    int GetDepthInMainChain(int& nHeightRet) const;
    int GetDepthInMainChain() const;
    bool IsInMainChain() const;
    int GetBlocksToMaturity() const;
    bool AcceptToMemoryPool(CTxDB& txdb, bool fCheckInputs=true);
    bool AcceptToMemoryPool();
};




//
// A transaction with a bunch of additional info that only the owner cares
// about.  It includes any unrecorded transactions needed to link it back
// to the block chain.
//
class CWalletTx : public CMerkleTx
{
public:
    vector<CMerkleTx> vtxPrev;
    map<string, string> mapValue;
    vector<pair<string, string> > vOrderForm;
    unsigned int fTimeReceivedIsTxTime;
    unsigned int nTimeReceived;  // time received by this node
    char fFromMe;
    string strFromAccount;
    vector<char> vfSpent;

    // memory only
    mutable char fDebitCached;
    mutable char fCreditCached;
    mutable char fAvailableCreditCached;
    mutable char fChangeCached;
    mutable int64 nDebitCached;
    mutable int64 nCreditCached;
    mutable int64 nAvailableCreditCached;
    mutable int64 nChangeCached;

    // memory only UI hints
    mutable unsigned int nTimeDisplayed;
    mutable int nLinesDisplayed;
    mutable char fConfirmedDisplayed;


    CWalletTx();
    CWalletTx(const CMerkleTx& txIn);

    CWalletTx(const CTransaction& txIn);
    void Init();

    IMPLEMENT_SERIALIZE
    (
        CWalletTx* pthis = const_cast<CWalletTx*>(this);
        if (fRead)
            pthis->Init();
        char fSpent = false;

        if (!fRead)
        {
            pthis->mapValue["fromaccount"] = pthis->strFromAccount;

            string str;
            foreach(char f, vfSpent)
            {
                str += (f ? '1' : '0');
                if (f)
                    fSpent = true;
            }
            pthis->mapValue["spent"] = str;
        }

        nSerSize += SerReadWrite(s, *(CMerkleTx*)this, nType, nVersion,ser_action);
        READWRITE(vtxPrev);
        READWRITE(mapValue);
        READWRITE(vOrderForm);
        READWRITE(fTimeReceivedIsTxTime);
        READWRITE(nTimeReceived);
        READWRITE(fFromMe);
        READWRITE(fSpent);

        if (fRead)
        {
            pthis->strFromAccount = pthis->mapValue["fromaccount"];

            if (mapValue.count("spent"))
                foreach(char c, pthis->mapValue["spent"])
                    pthis->vfSpent.push_back(c != '0');
            else
                pthis->vfSpent.assign(vout.size(), fSpent);
        }

        pthis->mapValue.erase("fromaccount");
        pthis->mapValue.erase("version");
        pthis->mapValue.erase("spent");
    )

    // marks certain txout's as spent
    // returns true if any update took place
    bool UpdateSpent(const vector<char>& vfNewSpent);
    void MarkDirty();
    void MarkSpent(unsigned int nOut);

    bool IsSpent(unsigned int nOut) const;

    int64 GetDebit() const;
    int64 GetCredit(bool fUseCache=true) const;
    int64 GetAvailableCredit(bool fUseCache=true) const;
    int64 GetChange() const;

    void GetAmounts(int64& nGeneratedImmature, int64& nGeneratedMature, list<pair<string /* address */, int64> >& listReceived,
                    list<pair<string /* address */, int64> >& listSent, int64& nFee, string& strSentAccount) const;

    void GetAccountAmounts(const string& strAccount, int64& nGenerated, int64& nReceived, 
                           int64& nSent, int64& nFee) const;

    bool IsFromMe() const;
    bool IsConfirmed() const;
    bool WriteToDisk();


    int64 GetTxTime() const;
    int GetRequestCount() const;

    void AddSupportingTransactions(CTxDB& txdb);

    bool AcceptWalletTransaction(CTxDB& txdb, bool fCheckInputs=true);
    bool AcceptWalletTransaction();

    void RelayWalletTransaction(CTxDB& txdb);
    void RelayWalletTransaction();
};




//
// A txdb record that contains the disk location of a transaction and the
// locations of transactions that spend its outputs.  vSpent is really only
// used as a flag, but having the location is very helpful for debugging.
//
class CTxIndex
{
public:
    CDiskTxPos pos;
    vector<CDiskTxPos> vSpent;

    CTxIndex();

    CTxIndex(const CDiskTxPos& posIn, unsigned int nOutputs);

    IMPLEMENT_SERIALIZE
    (
        if (!(nType & SER_GETHASH))
            READWRITE(nVersion);
        READWRITE(pos);
        READWRITE(vSpent);
    )

    void SetNull();
    bool IsNull();


    friend bool operator==(const CTxIndex& a, const CTxIndex& b)
    {
        return (a.pos    == b.pos &&
                a.vSpent == b.vSpent);
    }

    friend bool operator!=(const CTxIndex& a, const CTxIndex& b)
    {
        return !(a == b);
    }
    int GetDepthInMainChain() const;
};





//
// Nodes collect new transactions into a block, hash them into a hash tree,
// and scan through nonce values to make the block's hash satisfy proof-of-work
// requirements.  When they solve the proof-of-work, they broadcast the block
// to everyone and the block is added to the block chain.  The first transaction
// in the block is a special one that creates a new coin owned by the creator
// of the block.
//
// Blocks are appended to blk0001.dat files on disk.  Their location on disk
// is indexed by CBlockIndex objects in memory.
//
class CBlock
{
public:
    // header
    int nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    unsigned int nTime;
    unsigned int nBits;
    unsigned int nNonce;

    // network and disk
    vector<CTransaction> vtx;

    // memory only
    mutable vector<uint256> vMerkleTree;


    CBlock();

    IMPLEMENT_SERIALIZE
    (
        READWRITE(this->nVersion);
        nVersion = this->nVersion;
        READWRITE(hashPrevBlock);
        READWRITE(hashMerkleRoot);
        READWRITE(nTime);
        READWRITE(nBits);
        READWRITE(nNonce);

        // ConnectBlock depends on vtx being last so it can calculate offset
        if (!(nType & (SER_GETHASH|SER_BLOCKHEADERONLY)))
            READWRITE(vtx);
        else if (fRead)
            const_cast<CBlock*>(this)->vtx.clear();
    )

    void SetNull();
    bool IsNull() const;

    uint256 GetHash() const;
    int64 GetBlockTime() const;
    int GetSigOpCount() const;


    uint256 BuildMerkleTree() const;
    vector<uint256> GetMerkleBranch(int nIndex) const;

    static uint256 CheckMerkleBranch(uint256 hash, const vector<uint256>& vMerkleBranch, int nIndex);

    bool WriteToDisk(unsigned int& nFileRet, unsigned int& nBlockPosRet);

    bool ReadFromDisk(unsigned int nFile, unsigned int nBlockPos, bool fReadTransactions=true);



    void print() const;


    bool DisconnectBlock(CTxDB& txdb, CBlockIndex* pindex);
    bool ConnectBlock(CTxDB& txdb, CBlockIndex* pindex);
    bool ReadFromDisk(const CBlockIndex* pindex, bool fReadTransactions=true);
    bool SetBestChain(CTxDB& txdb, CBlockIndex* pindexNew);
    bool AddToBlockIndex(unsigned int nFile, unsigned int nBlockPos);
    bool CheckBlock() const;
    bool AcceptBlock();
};






//
// The block chain is a tree shaped structure starting with the
// genesis block at the root, with each block potentially having multiple
// candidates to be the next block.  pprev and pnext link a path through the
// main/longest chain.  A blockindex may have multiple pprev pointing back
// to it, but pnext will only point forward to the longest branch, or will
// be null if the block is not part of the longest chain.
//
class CBlockIndex
{
public:
    const uint256* phashBlock;
    CBlockIndex* pprev;
    CBlockIndex* pnext;
    unsigned int nFile;
    unsigned int nBlockPos;
    int nHeight;
    CBigNum bnChainWork;

    // block header
    int nVersion;
    uint256 hashMerkleRoot;
    unsigned int nTime;
    unsigned int nBits;
    unsigned int nNonce;


    CBlockIndex();

    CBlockIndex(unsigned int nFileIn, unsigned int nBlockPosIn, CBlock& block);

    CBlock GetBlockHeader() const;
    uint256 GetBlockHash() const;
    int64 GetBlockTime() const;
    CBigNum GetBlockWork() const;

    bool IsInMainChain() const;

    bool CheckIndex() const;

    bool EraseBlockFromDisk();

    enum { nMedianTimeSpan=11 };

    int64 GetMedianTimePast() const;
    int64 GetMedianTime() const;

    string ToString() const;
    void print() const;
};



//
// Used to marshal pointers into hashes for db storage.
//
class CDiskBlockIndex : public CBlockIndex
{
public:
    uint256 hashPrev;
    uint256 hashNext;

    CDiskBlockIndex();

    explicit CDiskBlockIndex(CBlockIndex* pindex);

    IMPLEMENT_SERIALIZE
    (
        if (!(nType & SER_GETHASH))
            READWRITE(nVersion);

        READWRITE(hashNext);
        READWRITE(nFile);
        READWRITE(nBlockPos);
        READWRITE(nHeight);

        // block header
        READWRITE(this->nVersion);
        READWRITE(hashPrev);
        READWRITE(hashMerkleRoot);
        READWRITE(nTime);
        READWRITE(nBits);
        READWRITE(nNonce);
    )

    uint256 GetBlockHash() const;


    string ToString() const;

    void print() const;
};








//
// Describes a place in the block chain to another node such that if the
// other node doesn't have the same branch, it can find a recent common trunk.
// The further back it is, the further before the fork it may be.
//
class CBlockLocator
{
protected:
    vector<uint256> vHave;
public:

    CBlockLocator();

    explicit CBlockLocator(const CBlockIndex* pindex);

    explicit CBlockLocator(uint256 hashBlock);

    IMPLEMENT_SERIALIZE
    (
        if (!(nType & SER_GETHASH))
            READWRITE(nVersion);
        READWRITE(vHave);
    )

    void SetNull();
    bool IsNull();

    void Set(const CBlockIndex* pindex);

    int GetDistanceBack();
    CBlockIndex* GetBlockIndex();
    uint256 GetBlockHash();
    int GetHeight();
};






//
// Private key that includes an expiration date in case it never gets used.
//
class CWalletKey
{
public:
    CPrivKey vchPrivKey;
    int64 nTimeCreated;
    int64 nTimeExpires;
    string strComment;
    //// todo: add something to note what created it (user, getnewaddress, change)
    ////   maybe should have a map<string, string> property map

    CWalletKey(int64 nExpires=0);

    IMPLEMENT_SERIALIZE
    (
        if (!(nType & SER_GETHASH))
            READWRITE(nVersion);
        READWRITE(vchPrivKey);
        READWRITE(nTimeCreated);
        READWRITE(nTimeExpires);
        READWRITE(strComment);
    )
};






//
// Account information.
// Stored in wallet with key "acc"+string account name
//
class CAccount
{
public:
    vector<unsigned char> vchPubKey;

    CAccount();

    void SetNull();

    IMPLEMENT_SERIALIZE
    (
        if (!(nType & SER_GETHASH))
            READWRITE(nVersion);
        READWRITE(vchPubKey);
    )
};



//
// Internal transfers.
// Database key is acentry<account><counter>
//
class CAccountingEntry
{
public:
    string strAccount;
    int64 nCreditDebit;
    int64 nTime;
    string strOtherAccount;
    string strComment;

    CAccountingEntry();

    void SetNull();

    IMPLEMENT_SERIALIZE
    (
        if (!(nType & SER_GETHASH))
            READWRITE(nVersion);
        // Note: strAccount is serialized as part of the key, not here.
        READWRITE(nCreditDebit);
        READWRITE(nTime);
        READWRITE(strOtherAccount);
        READWRITE(strComment);
    )
};









//
// Alerts are for notifying old versions if they become too obsolete and
// need to upgrade.  The message is displayed in the status bar.
// Alert messages are broadcast as a vector of signed data.  Unserializing may
// not read the entire buffer if the alert is for a newer version, but older
// versions can still relay the original data.
//
class CUnsignedAlert
{
public:
    int nVersion;
    int64 nRelayUntil;      // when newer nodes stop relaying to newer nodes
    int64 nExpiration;
    int nID;
    int nCancel;
    set<int> setCancel;
    int nMinVer;            // lowest version inclusive
    int nMaxVer;            // highest version inclusive
    set<string> setSubVer;  // empty matches all
    int nPriority;

    // Actions
    string strComment;
    string strStatusBar;
    string strReserved;

    IMPLEMENT_SERIALIZE
    (
        READWRITE(this->nVersion);
        nVersion = this->nVersion;
        READWRITE(nRelayUntil);
        READWRITE(nExpiration);
        READWRITE(nID);
        READWRITE(nCancel);
        READWRITE(setCancel);
        READWRITE(nMinVer);
        READWRITE(nMaxVer);
        READWRITE(setSubVer);
        READWRITE(nPriority);

        READWRITE(strComment);
        READWRITE(strStatusBar);
        READWRITE(strReserved);
    )

    void SetNull();

    string ToString() const;
    void print() const;
};

class CAlert : public CUnsignedAlert
{
public:
    vector<unsigned char> vchMsg;
    vector<unsigned char> vchSig;

    CAlert();

    IMPLEMENT_SERIALIZE
    (
        READWRITE(vchMsg);
        READWRITE(vchSig);
    )

    void SetNull();

    bool IsNull() const;

    uint256 GetHash() const;

    bool IsInEffect() const;

    bool Cancels(const CAlert& alert) const;

    bool AppliesTo(int nVersion, string strSubVerIn) const;

    bool AppliesToMe() const;

    bool RelayTo(CNode* pnode) const;

    bool CheckSignature();

    bool ProcessAlert();
};










extern map<uint256, CTransaction> mapTransactions;
extern map<uint256, CWalletTx> mapWallet;
extern vector<uint256> vWalletUpdated;
extern CCriticalSection cs_mapWallet;
extern map<vector<unsigned char>, CPrivKey> mapKeys;
extern map<uint160, vector<unsigned char> > mapPubKeys;
extern CCriticalSection cs_mapKeys;
extern CKey keyUser;
