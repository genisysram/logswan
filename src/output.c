/*****************************************************************************/
/*                                                                           */
/* Logswan (c) by Frederic Cambus 2015                                       */
/* https://github.com/fcambus/logswan                                        */
/*                                                                           */
/* Created:      2015/05/31                                                  */
/* Last Updated: 2015/06/24                                                  */
/*                                                                           */
/* Logswan is released under the BSD 3-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#include <GeoIP.h>
#include <jansson.h>

#include "logswan.h"
#include "results.h"

char *output(Results results) {
	char *methods[] = {
		"OPTIONS",
		"GET",
		"HEAD",
		"POST",
		"PUT",
		"DELETE",
		"TRACE",
		"CONNECT",
		"PATCH"
	};

	char *protocols[] = { 
		"HTTP/1.0",
		"HTTP/1.1"
	};

	json_t *jsonObject = json_object();
	json_t *hitsObject = json_object();
	json_t *countriesArray = json_array();
	json_t *hoursArray = json_array();
	json_t *httpStatusArray = json_array();
	json_t *methodsArray = json_array();
	json_t *protocolsArray = json_array();

	for (int loop=0; loop<255; loop++) {
		if (results.countries[loop]) {
			json_array_append_new(countriesArray, json_pack("{s:s, s:s, s:i}", "data", GeoIP_code_by_id(loop), "name", GeoIP_name_by_id(loop), "hits", results.countries[loop]));
		}
	}

	for (int loop=0; loop<24; loop++) {
		if (results.hours[loop]) {
			json_array_append_new(hoursArray, json_pack("{s:i, s:i}", "data", loop, "hits", results.hours[loop]));
		}
	}

	for (int loop=0; loop<512; loop++) {
		if (results.httpStatus[loop]) {
			json_array_append_new(httpStatusArray, json_pack("{s:i, s:i}", "data", loop, "hits", results.httpStatus[loop]));
		}
	}

	for (int loop=0; loop<9; loop++) {
		if (results.methods[loop]) {
			json_array_append_new(methodsArray, json_pack("{s:s, s:i}", "data", methods[loop], "hits", results.methods[loop]));
		}
	}

	for (int loop=0; loop<2; loop++) {
		if (results.protocols[loop]) {
			json_array_append_new(protocolsArray, json_pack("{s:s, s:i}", "data", protocols[loop], "hits", results.protocols[loop]));
		}
	}

	json_object_set_new(hitsObject, "ipv4", json_integer(results.hitsIPv4));
	json_object_set_new(hitsObject, "ipv6", json_integer(results.hitsIPv6));
	json_object_set_new(hitsObject, "total", json_integer(results.hits));
	json_object_set_new(hitsObject, "countries", countriesArray);
	json_object_set_new(hitsObject, "hours", hoursArray);
	json_object_set_new(hitsObject, "http_status", httpStatusArray);
	json_object_set_new(hitsObject, "methods", methodsArray);
	json_object_set_new(hitsObject, "protocols", protocolsArray);

	json_object_set_new(jsonObject, "date", json_string(results.timeStamp));
	json_object_set_new(jsonObject, "generator", json_string(VERSION));
	json_object_set_new(jsonObject, "file_size", json_integer(results.fileSize));
	json_object_set_new(jsonObject, "processed_lines", json_integer(results.processedLines));
	json_object_set_new(jsonObject, "invalid_lines", json_integer(results.invalidLines));
	json_object_set_new(jsonObject, "bandwidth", json_integer(results.bandwidth));
	json_object_set_new(jsonObject, "runtime", json_real(results.runtime));
	json_object_set_new(jsonObject, "hits", hitsObject);

	return json_dumps(jsonObject, JSON_INDENT(3) | JSON_PRESERVE_ORDER);
}
