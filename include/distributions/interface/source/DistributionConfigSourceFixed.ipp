/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 * Contains declarations for the DistributionConfigSourceFixed Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_FIXED_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_FIXED_IPP_H

#include "DistributionConfigFixed.h"

namespace cupcfd
{
	namespace distributions
	{
		// === Constructors/Deconstructors ===
	
		template <class I, class T>
		DistributionConfigSourceFixed<I,T>::DistributionConfigSourceFixed()
		:DistributionConfigSource<I,T>()
		{
		
		}
		
		template <class I, class T>
		DistributionConfigSourceFixed<I,T>::DistributionConfigSourceFixed(const DistributionConfigSourceFixed<I,T>& source)
		{
		
		}
		
		template <class I, class T>
		DistributionConfigSourceFixed<I,T>::~DistributionConfigSourceFixed()
		{
		
		}
		
		template <class I, class T>
		void DistributionConfigSourceFixed<I,T>::operator=(const DistributionConfigSourceFixed<I,T>& source)
		{
		
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceFixed<I,T>::getFixedValue(T * value)
		{
		
		}
						
		template <class I, class T>
		DistributionConfigSourceFixed<I,T> * DistributionConfigSourceFixed<I,T>::clone()
		{
		
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceFixed<I,T>::buildDistributionConfig(DistributionConfig<I,T> ** distConfig)
		{
			cupcfd::error::eCodes status;
			T fixedValue;
			
			status = this->getFixedValue(&fixedValue);
			
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			*distConfig = new DistributionConfigFixed<I,T>(fixedValue);
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
