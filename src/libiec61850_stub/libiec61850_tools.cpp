/*
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDClient.
 *
 *  IEDClient is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDClient is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDClient.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#include "libiec61850_tools.hpp"

extern "C" {
    #include <iec61850_common.h>
    #include <mms_client_connection.h>
}

namespace Tools
{
    void Lib61850_Tools::getModelDump()
    {
        int maxPduSize = 2000;
        int tcpPort = 102;
        char hostname[] = "127.0.0.1";

        MmsConnection con = MmsConnection_create();

        MmsError error = MMS_ERROR_NONE;

        /* Set maximum MMS PDU size (local detail) to 2000 byte */
        MmsConnection_setLocalDetail(con, maxPduSize);

        if (!MmsConnection_connect(con, &error, hostname, tcpPort)) {
            printf("MMS connect failed!\n");
        }
        printf("MMS connected.\n");

        bool identifyDevice = true;
        if (identifyDevice) {
            MmsServerIdentity* identity = MmsConnection_identify(con, &error);
            if (identity != NULL) {
                printf("\nServer identity:\n----------------\n");
                printf("  vendor:\t%s\n", identity->vendorName);
                printf("  model:\t%s\n", identity->modelName);
                printf("  revision:\t%s\n", identity->revision);
            }
        }

        bool readDeviceList = true;
        if (readDeviceList) {
            printf("\nDomains present on server:\n--------------------------\n");
            LinkedList nameList = MmsConnection_getDomainNames(con, &error);
            LinkedList_printStringList(nameList);
            LinkedList_destroy(nameList);
        }

        bool getDeviceDirectory = true;
        if (getDeviceDirectory) {
            char domainName[] = "LD1";

            LinkedList variableList = MmsConnection_getDomainVariableNames(con, &error, domainName);
            if (variableList) {
                LinkedList element = LinkedList_getNext(variableList);

                printf("\nMMS domain variables for domain %s\n", domainName);

                while (element != NULL) {
                    char* name = (char*) element->data;

                    printf("  %s\n", name);

                    element = LinkedList_getNext(element);
                }

                LinkedList_destroy(variableList);
            }
            else {
                printf("\nFailed to read domain directory (error=%d)\n", error);
            }

            variableList = MmsConnection_getDomainJournals(con, &error, domainName);
            if (variableList) {

                LinkedList element = variableList;

                printf("\nMMS journals for domain %s\n", domainName);

                while ((element = LinkedList_getNext(element)) != NULL) {
                    char* name = (char*) element->data;

                    printf("  %s\n", name);
                }

                LinkedList_destroy(variableList);
            }
            else {
                printf("\nFailed to read domain journals (error=%d)\n", error);
            }
        }

        /*
        if (readVariable) {
            if (readWriteHasDomain) {

                MmsValue* result;

                if (componentName == NULL) {
                    if (arrayIndex == -1) {
                        result = MmsConnection_readVariable(con, &error, domainName, variableName);
                    }
                    else {
                        result = MmsConnection_readSingleArrayElementWithComponent(con, &error, domainName, variableName, arrayIndex, NULL);
                    }
                }
                else {
                    if (arrayIndex == -1) {
                        result = MmsConnection_readVariableComponent(con, &error, domainName, variableName, componentName);
                    }
                    else {
                        result = MmsConnection_readSingleArrayElementWithComponent(con, &error, domainName, variableName, arrayIndex, componentName);
                    }
                }

                if (error != MMS_ERROR_NONE) {
                    printf("Reading variable failed: (ERROR %i)\n", error);
                }
                else {
                    printf("Read SUCCESS\n");

                    if (result != NULL) {
                        char outbuf[1024];

                        MmsValue_printToBuffer(result, outbuf, 1024);

                        printf("%s\n", outbuf);

                        MmsValue_delete(result);
                    }
                    else
                        printf("result: NULL\n");
                }

            }
            else
                printf("Reading VMD scope variable not yet supported!\n");
        }

        if (readVariableList) {
            if (readWriteHasDomain) {
                MmsValue* variables = MmsConnection_readNamedVariableListValues(con, &error, domainName, variableName, true);

                if (error != MMS_ERROR_NONE) {
                    printf("Reading variable failed: (ERROR %i)\n", error);
                }
                else {
                    printf("Read SUCCESS\n");
                }
            }
            else
                printf("Reading VMD scope variable list not yet supported!\n");
        }
        */
    }
}