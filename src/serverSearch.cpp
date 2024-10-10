/* --------------------------------------------------------------------------
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
 serverSearch.cpp
 
 search.wsdl server
----------------------------------------------------------------------------- */


#include "soapSearchBindingService.h"
#include "onvif_impl.h"

int SearchBindingService::GetServiceCapabilities(_tse__GetServiceCapabilities *tse__GetServiceCapabilities, _tse__GetServiceCapabilitiesResponse &tse__GetServiceCapabilitiesResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	ServiceContext* ctx = (ServiceContext*)this->soap->user;
	tse__GetServiceCapabilitiesResponse.Capabilities = ctx->getSearchServiceCapabilities(this->soap);	
	return SOAP_OK;
}

int SearchBindingService::GetRecordingSummary(_tse__GetRecordingSummary *tse__GetRecordingSummary, _tse__GetRecordingSummaryResponse &tse__GetRecordingSummaryResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::GetRecordingInformation(_tse__GetRecordingInformation *tse__GetRecordingInformation, _tse__GetRecordingInformationResponse &tse__GetRecordingInformationResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::GetMediaAttributes(_tse__GetMediaAttributes *tse__GetMediaAttributes, _tse__GetMediaAttributesResponse &tse__GetMediaAttributesResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::FindRecordings(_tse__FindRecordings *tse__FindRecordings, _tse__FindRecordingsResponse &tse__FindRecordingsResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::GetRecordingSearchResults(_tse__GetRecordingSearchResults *tse__GetRecordingSearchResults, _tse__GetRecordingSearchResultsResponse &tse__GetRecordingSearchResultsResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::FindEvents(_tse__FindEvents *tse__FindEvents, _tse__FindEventsResponse &tse__FindEventsResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::GetEventSearchResults(_tse__GetEventSearchResults *tse__GetEventSearchResults, _tse__GetEventSearchResultsResponse &tse__GetEventSearchResultsResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::FindPTZPosition(_tse__FindPTZPosition *tse__FindPTZPosition, _tse__FindPTZPositionResponse &tse__FindPTZPositionResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::GetPTZPositionSearchResults(_tse__GetPTZPositionSearchResults *tse__GetPTZPositionSearchResults, _tse__GetPTZPositionSearchResultsResponse &tse__GetPTZPositionSearchResultsResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::GetSearchState(_tse__GetSearchState *tse__GetSearchState, _tse__GetSearchStateResponse &tse__GetSearchStateResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::EndSearch(_tse__EndSearch *tse__EndSearch, _tse__EndSearchResponse &tse__EndSearchResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::FindMetadata(_tse__FindMetadata *tse__FindMetadata, _tse__FindMetadataResponse &tse__FindMetadataResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}

int SearchBindingService::GetMetadataSearchResults(_tse__GetMetadataSearchResults *tse__GetMetadataSearchResults, _tse__GetMetadataSearchResultsResponse &tse__GetMetadataSearchResultsResponse) 
{	
#ifdef DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	return SOAP_OK;
}


