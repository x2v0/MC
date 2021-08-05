#include "pch.h"
#include "ElementFactory.h"
using namespace RO::Dicom;
using namespace Vr;

std::shared_ptr<Element> ElementFactory::MakeElement(UINT32 tag, VR vr)
{
   std::shared_ptr<Element> v;
   if (vr == AE)
      v = std::make_shared<ElementString>(tag, vr);
   else if (vr == AS)
      v = std::make_shared<ElementAS>(tag, vr);
   else if (vr == AT)
      v = std::make_shared<ElementAT>(tag, vr);
   else if (vr == CS)
      v = std::make_shared<ElementCS>(tag, vr);
   else if (vr == DA)
      v = std::make_shared<ElementDA>(tag, vr);
   else if (vr == TM)
      v = std::make_shared<ElementTM>(tag, vr);
   else if (vr == DT)
      v = std::make_shared<ElementDT>(tag, vr); // user will parse text for numerical value himself
   else if (vr == DS || vr == IS || vr == LO || vr == LT || vr == PN || vr == SH || vr == ST)
      v = std::make_shared<ElementString>(tag, vr); // user will parse text for numerical value himself
   else if (vr == OB || vr == OF || vr == OW || vr == UN || vr == UT)
      v = std::make_shared<ElementOtherString>(tag, vr);
   else if (vr == FL)
      v = std::make_shared<ElementFL>(tag, vr);
   else if (vr == FD)
      v = std::make_shared<ElementFD>(tag, vr);
   else if (vr == SL)
      v = std::make_shared<ElementSL>(tag, vr);
   else if (vr == SS)
      v = std::make_shared<ElementSS>(tag, vr);
   else if (vr == UI)
      v = std::make_shared<ElementUI>(tag, vr);
   else if (vr == UL)
      v = std::make_shared<ElementUL>(tag, vr);
   else if (vr == US)
      v = std::make_shared<ElementUS>(tag, vr);
   else if (vr == SQ)
      v = std::make_shared<ElementSQ>(tag, vr);
   else
      throw std::exception("unknown representatrion");
   return v;
}

