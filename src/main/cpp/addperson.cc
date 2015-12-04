#include "addressbook.piqi.pb.h"
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/type_resolver.h>
#include <google/protobuf/util/type_resolver_util.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

// Wouldn't normally do this, but this is just a silly test...
using namespace std;
using namespace google::protobuf;
using namespace google::protobuf::util;

static const char kTypeUrlPrefix[] = "tutorial";

static string GetTypeUrl(const Descriptor* message) {
    return string(kTypeUrlPrefix) + "/" + message->full_name();
}

static void AddressBookFromJsonFile( const char *path, tutorial::AddressBook &book ){
    // Read json string from file.
    ifstream input(path);
    std::stringstream json;
    json << input.rdbuf();

    if( input.fail() ){
        throw runtime_error(string("Failed to read file: ") + path);
    }

    // Get binary string from json string.
    unique_ptr<TypeResolver> resolver(
        NewTypeResolverForDescriptorPool(
            kTypeUrlPrefix, DescriptorPool::generated_pool()));

    string pb;
    auto status = JsonToBinaryString(
        resolver.get(),
        GetTypeUrl(book.GetDescriptor()),
        json.str(),
        &pb);

    if( status != Status::OK ){
        throw runtime_error("Failed to get binary from JSON\n");
    }

    // Get AddressBook from binary string.
    bool parse_success = book.ParseFromString(pb);

    if( !parse_success ){
        throw runtime_error("Failed to parse AddressBook from binary\n");
    }
}

static void JsonFileFromAddressBook( const char *path, tutorial::AddressBook &book ){
    unique_ptr<TypeResolver> resolver(
        NewTypeResolverForDescriptorPool(
            kTypeUrlPrefix, DescriptorPool::generated_pool()));

    string result;
    JsonOptions options;
    options.add_whitespace = true;

    auto status = BinaryToJsonString(
        resolver.get(),
        GetTypeUrl(book.GetDescriptor()),
        book.SerializeAsString(),
        &result,
        options);

    if( status != Status::OK ){
        throw runtime_error("Failed to get json from message");
    }

    fstream json_output(path, ios::out | ios::binary);
    json_output << result;

    if( json_output.fail() ){
        throw runtime_error(string("Failed to write file: ") + path);
    }
}

int main(int argc, char **argv){
    if( argc != 2 ){
        cerr << "Please provide path to json file containing AddressBook\n";
        exit(1);
    }
    const char * json_path = argv[1];

    tutorial::AddressBook book;
    AddressBookFromJsonFile(json_path, book);

    cout << "Parsed successfully, adding...\n";
    auto * newp = book.add_person();
    newp->set_name("Silly Person");
    newp->set_id(book.person_size());

    for ( auto & p : book.person() ){
        cout << "Person name: " << p.name() << endl;
        cout << "Person id: " << p.id() << endl;
    }

    cout << "Serializing to json file\n";
    JsonFileFromAddressBook(json_path, book);

    return 0;
}
