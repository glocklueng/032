  
#include "stdafx.h"

namespace Multi
{
	 int get_err_no()
	 {
        int temp;
        _get_errno(&temp);
        return temp;
    }
}
