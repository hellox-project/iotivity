/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef RAML_H
#define RAML_H

#include <map>
#include <list>
#include <string>
#include "AbstractParam.h"
#include "UriParameter.h"
#include "QueryParameter.h"
#include "FormParameter.h"
#include "Header.h"

#include "RequestResponseBody.h"
#include "RamlResource.h"
#include "ActionType.h"
#include "Action.h"
#include "Response.h"
#include "Schema.h"
#include "IncludeResolver.h"

#include "DocumentationItem.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/exceptions.h"
#include "RamlExceptions.h"
#include "Utils.h"
#include "cJSON.h"


namespace RAML
{
    class Raml
    {
        public:
            virtual std::string getTitle() const;
            virtual void setTitle(const std::string &title);

            virtual std::string getVersion() const;
            virtual void setVersion(const std::string &version);

            virtual std::string getBaseUri() const;
            virtual void setBaseUri(const std::string &baseUri);

            virtual std::list<std::string> const &getProtocols() const;
            virtual void setProtocol(const std::string &protocol);


            virtual std::map<std::string, UriParameterPtr> const &getBaseUriParameters() const;
            virtual void setBaseUriParameter(const std::string &paramName, const UriParameterPtr &uriParameter);

            virtual std::string getMediaType() const;
            virtual void setMediaType(const std::string &mediaType);

            virtual std::list<std::pair<std::string, SchemaPtr> > const &getSchemas() const;
            virtual void setSchema(const std::string &schemaName, const SchemaPtr &schema);

            virtual std::list<std::pair<std::string, RamlResourcePtr> > const &getResourceTypes() const;
            virtual void setResourceType(const std::string &typeName, const RamlResourcePtr &resourceType);

            virtual std::list<std::pair<std::string, ActionPtr> > const &getTraits() const;
            virtual void setTrait(const std::string &traitName, const ActionPtr &trait);

            virtual RamlResourcePtr getResource(const std::string &resourceName);
            virtual std::map<std::string, RamlResourcePtr> const &getResources() const;
            virtual void setResource(const std::string &resourceKey, const RamlResourcePtr &resource);

            virtual void setDocumentationItem(const std::shared_ptr<DocumentationItem> &documentationItem);
            virtual std::list<std::shared_ptr<DocumentationItem> > const &getDocumentation() const;

            void readRamlFromYaml(const YAML::Node &yamlNode);
            Raml() : m_includeResolver(std::make_shared<IncludeResolver>()) {}
            Raml(const std::string &fileLocation,
                 const std::string &ramlName) : m_includeResolver(std::make_shared<IncludeResolver>(fileLocation))
            {
                try
                {
                    YAML::Node yamlRootNode = YAML::LoadFile(fileLocation + ramlName);
                    readRamlFromYaml(yamlRootNode);
                }
                catch (YAML::ParserException &e)
                {
                    throw RamlParserException(e.mark, e.msg);
                }
                catch (YAML::RepresentationException &e)
                {
                    throw RamlRepresentationException(e.mark, e.msg);
                }
                catch (YAML::BadFile &e)
                {
                    throw RamlBadFile(e.mark, e.msg);
                }
                catch (JsonException &e)
                {
                    throw;
                }
            }
        private:
            std::string m_title;
            std::string m_version;
            std::string m_baseUri;
            std::list<std::string> m_protocols;
            std::map<std::string, UriParameterPtr> m_baseUriParameters;
            std::string m_mediaType;
            std::list <std::pair<std::string, SchemaPtr> > m_schemas;
            std::list <std::pair<std::string, RamlResourcePtr> > m_resourceTypes;
            std::list <std::pair<std::string, ActionPtr> > m_traits;
            std::map<std::string, RamlResourcePtr> m_resources;
            std::list<std::shared_ptr<DocumentationItem> > m_documentation;
            IncludeResolverPtr m_includeResolver;
    };

    typedef std::shared_ptr<Raml> RamlPtr;

}
#endif