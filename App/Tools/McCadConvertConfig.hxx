#ifndef MCCADCONVERTCONFIG_HXX
#define MCCADCONVERTCONFIG_HXX

#include <string>
#include <TCollection_AsciiString.hxx>
#include <map>



enum McCadGeom_Unit {
    McCadGeom_CM = 0,
    McCadGeom_MM = 1,
    McCadGeom_Inch = 2
};

using namespace std;

class McCadConvertConfig
{

public:

    void* operator new(size_t,void* anAddress)
      {
        return anAddress;
      }
    void* operator new(size_t size)
      {
        return Standard::Allocate(size);
      }
    void  operator delete(void *anAddress)
      {
        if (anAddress) Standard::Free((Standard_Address&)anAddress);
      }

    McCadConvertConfig();

private:

    static bool m_bWriteCollisionFile;           /**< Whether write collision file or not */
    static bool m_bVoidGenerate;                 /**< Whether generate the void */

    static double m_dTolerance;                  /**< Tolerence, the value less than tolerence will be treat as zero*/
    static double m_dMinIptSolidVol;             /**< The minimum volume of input solid */
    static double m_dMaxDiscLength;              /**< The maximum length of cell discription */
    static double m_dMinDecomFaceArea;
    static double m_dMinVoidVol;
    static double m_dAngleTolerance;             /**< The angle toleranve between two direction */

    static unsigned int m_iVoidDecomposeDepth;   /**< The depth of void decomposition */
    static unsigned int m_iInitCellNum;          /**< Initial cell number */
    static unsigned int m_iInitSurfNum;          /**< Initial surface number */
    static double m_fXResolution;                /**< Resolution of X axis */
    static double m_fYResolution;                /**< Resolution of Y axis */
    static double m_fRResolution;                /**< R direction */

    static unsigned int m_iInitVoidBoxNum;       /**< Initial number of void boxes, n means n*n*n(x*y*z) void boxes */

    static unsigned int m_iMaxSmplPntNum;        /**< Max sample point number */
    static unsigned int m_iMinSmplPntNum;        /**< Min sample point number */

    static TCollection_AsciiString m_strInputFileName;   /**< Input file name */
    static TCollection_AsciiString m_strMatFile;         /**< Material file */
    static TCollection_AsciiString m_strDirectory;       /**< Working directory */

    static McCadGeom_Unit m_units;                       /**< Units of CAD model*/
    static map<TCollection_AsciiString,Standard_Integer> m_SurfSeq;

public:

    static void MissmatchMessage(TCollection_AsciiString &keyword, TCollection_AsciiString &parameter);
    static void SetSurfSequ();
    void AssignPrmt(const TCollection_AsciiString StringLine);

    Standard_EXPORT static Standard_Integer GetSurfSequNum(TCollection_AsciiString SurfSymb);

    Standard_EXPORT static bool ReadPrmt(const TCollection_AsciiString InputFileName);
    Standard_EXPORT static bool WriteCollisionFile(){return m_bWriteCollisionFile;};
    Standard_EXPORT static bool GenerateVoid(){return m_bVoidGenerate;};

    Standard_EXPORT static double GetTolerence(){return m_dTolerance;};
    Standard_EXPORT static double GetAngleTolerance(){return m_dAngleTolerance;};
    Standard_EXPORT static double GetMinIptSolidVod(){return m_dMinIptSolidVol;};
    Standard_EXPORT static double GetMaxDiscLength(){return m_dMaxDiscLength;};
    Standard_EXPORT static double GetMinDecomFaceArea(){return m_dMinDecomFaceArea;};
    Standard_EXPORT static double GetMinVoidVol(){return m_dMinVoidVol;};

    Standard_EXPORT static unsigned int GetVoidDecomposeDepth(){return m_iVoidDecomposeDepth;};
    Standard_EXPORT static unsigned int GetInitCellNum(){return m_iInitCellNum;};
    Standard_EXPORT static unsigned int GetInitSurfNum(){return m_iInitSurfNum;};
    Standard_EXPORT static double GetXResolution(){return m_fXResolution;};
    Standard_EXPORT static double GetYResolution(){return m_fYResolution;};
    Standard_EXPORT static double GetRResolution(){return m_fRResolution;};
    Standard_EXPORT static unsigned int GetMaxSmplPntNum(){return m_iMaxSmplPntNum;};
    Standard_EXPORT static unsigned int GetMinSmplPntNum(){return m_iMinSmplPntNum;};
    Standard_EXPORT static unsigned int GetInitVoidBoxNum(){return m_iInitVoidBoxNum;};

    Standard_EXPORT static TCollection_AsciiString GetInputFileName(){return m_strInputFileName;};
    Standard_EXPORT static TCollection_AsciiString GetMatFile(){return m_strMatFile;};
    Standard_EXPORT static TCollection_AsciiString GetDirectory(){return m_strDirectory;};

    Standard_EXPORT static void SetMinMaxSmplPntNum(int iMinPnt,int iMaxPnt);

};

#endif // MCCADVOIDPRMT_HXX
