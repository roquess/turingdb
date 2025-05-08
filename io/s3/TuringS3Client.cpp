#include "TuringS3Client.h"

#include <filesystem>
#include <iostream>
#include <fstream>

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3-crt/S3CrtClient.h>
#include <aws/s3-crt/S3CrtErrors.h>
#include <aws/s3-crt/ClientConfiguration.h>
#include <aws/s3-crt/model/CreateBucketRequest.h>
#include <aws/s3-crt/model/ListObjectsV2Request.h>
#include <aws/s3-crt/model/DeleteBucketRequest.h>
#include <aws/s3-crt/model/PutObjectRequest.h>
#include <aws/s3-crt/model/GetObjectRequest.h>
#include <aws/core/platform/FileSystem.h>

#include <spdlog/spdlog.h>

namespace S3 {

TuringS3Client::TuringS3Client()
{
	init();
}

TuringS3Client::~TuringS3Client() {
	Aws::ShutdownAPI(*_options);

}

void TuringS3Client::init() {
	_options = std::make_unique<Aws::SDKOptions>();
	_options->loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Trace;
	Aws::InitAPI(*_options);

	clientConfig = std::make_unique<Aws::S3Crt::ClientConfiguration>();
	clientConfig->region = "eu-west-2";

	//All client config options need to changed before here
	_client = Aws::MakeShared<Aws::S3Crt::S3CrtClient>("S3Client",*clientConfig);
}

void TuringS3Client::listBuckets() {
	auto outcome = _client->ListBuckets();

	if (!outcome.IsSuccess()) {
		std::cerr << "Failed with error: " << outcome.GetError() << std::endl;
	} else {
		std::cout << "Found " << outcome.GetResult().GetBuckets().size() << " buckets\n";
		for (auto &&b: outcome.GetResult().GetBuckets()) {
			std::cout << b.GetName() << std::endl;
		}
	}
}

bool TuringS3Client::createBucket(const Aws::String& bucketName) {
	Aws::S3Crt::Model::CreateBucketRequest request;
	request.SetBucket(bucketName);

	if (clientConfig->region != "us-east-1") {
        Aws::S3Crt::Model::CreateBucketConfiguration createBucketConfig;
        createBucketConfig.SetLocationConstraint(
                Aws::S3Crt::Model::BucketLocationConstraintMapper::GetBucketLocationConstraintForName(
                        clientConfig->region));
        request.SetCreateBucketConfiguration(createBucketConfig);
	}

	Aws::S3Crt::Model::CreateBucketOutcome outcome = _client->CreateBucket(request);
	if (!outcome.IsSuccess()) {
		const auto& err = outcome.GetError();
		std::cerr << "Error: createBucket: " <<
			err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
	} else {
		std::cout << "Created bucket " << bucketName <<
			" in the specified AWS Region." << std::endl;
	}

	return outcome.IsSuccess();
}

bool TuringS3Client::listKeys(const std::string& bucketName,std::vector<std::string>& keyResults, const std::string& prefix) {
	spdlog::info("listing keys with prefix:{}", prefix);
	
	Aws::S3Crt::Model::ListObjectsV2Request request;
	request.WithBucket(bucketName).WithPrefix(prefix);

	Aws::String continuationToken; // Used for pagination.
	Aws::Vector<Aws::S3Crt::Model::Object> allObjects;

	do {
		if (!continuationToken.empty()) {
			request.SetContinuationToken(continuationToken);
		}

		auto outcome = _client->ListObjectsV2(request);

		if (!outcome.IsSuccess()) {
			std::cerr << "Error: listObjects: " <<
				outcome.GetError().GetMessage() << std::endl;
			return false;
		} else {
			Aws::Vector<Aws::S3Crt::Model::Object> objects =
				outcome.GetResult().GetContents();

			allObjects.insert(allObjects.end(), objects.begin(), objects.end());
			continuationToken = outcome.GetResult().GetNextContinuationToken();
		}
	} while (!continuationToken.empty());
	//std::cout << allObjects.size() << " object(s) found:" << std::endl;

	for (const auto &object: allObjects) {
		keyResults.push_back(object.GetKey());
		//std::cout << "  " << keyResults.back() << std::endl;
	}

	return true;

}

std::string extractFileNameFromKey(const Aws::String& commonPrefix) {
	size_t lastSlash = commonPrefix.find_last_of('/');

	return commonPrefix.substr(lastSlash+1);
}

bool TuringS3Client::listFiles(const std::string& bucketName,std::vector<std::string>& keyResults, const std::string& prefix) {
	spdlog::info("listing files with prefix:{}", prefix);

	Aws::S3Crt::Model::ListObjectsV2Request request;
	request.WithBucket(bucketName).WithPrefix(prefix).WithDelimiter("/");

	Aws::String continuationToken; // Used for pagination.
	Aws::Vector<Aws::S3Crt::Model::Object> allObjects;

	do {
		if (!continuationToken.empty()) {
			request.SetContinuationToken(continuationToken);
		}

		auto outcome = _client->ListObjectsV2(request);

		if (!outcome.IsSuccess()) {
			std::cerr << "Error: listObjects: " <<
				outcome.GetError().GetMessage() << std::endl;
			return false;
		} else {
			Aws::Vector<Aws::S3Crt::Model::Object> objects =
				outcome.GetResult().GetContents();

			allObjects.insert(allObjects.end(), objects.begin(), objects.end());
			continuationToken = outcome.GetResult().GetNextContinuationToken();
		}
	} while (!continuationToken.empty());

	for (const auto &object: allObjects) {
		keyResults.push_back(extractFileNameFromKey(object.GetKey()));
	}

	return true;
}

std::string commonPrefixToFolderName(const Aws::String& commonPrefix) {
	size_t lastSlash = commonPrefix.find_last_of('/');
	size_t secondLastSlash = commonPrefix.find_last_of('/', lastSlash - 1);

	return commonPrefix.substr(secondLastSlash + 1, lastSlash - secondLastSlash - 1);
}

bool TuringS3Client::listFolders(const std::string& bucketName,std::vector<std::string>& folderResults, const std::string& prefix) {
	spdlog::info("listing folders with prefix:{}", prefix);
	
	Aws::S3Crt::Model::ListObjectsV2Request request;
	request.WithBucket(bucketName).WithPrefix(prefix).WithDelimiter("/");

	Aws::String continuationToken; // Used for pagination.
	Aws::Vector<Aws::S3Crt::Model::CommonPrefix> allFolders;

	do {
		if (!continuationToken.empty()) {
			request.SetContinuationToken(continuationToken);
		}

		auto outcome = _client->ListObjectsV2(request);

		if (!outcome.IsSuccess()) {
			std::cerr << "Error listing folders : " <<
				outcome.GetError().GetMessage() << std::endl;
			return false;
		} else {
			Aws::Vector<Aws::S3Crt::Model::CommonPrefix> folders =
				outcome.GetResult().GetCommonPrefixes();

			allFolders.insert(allFolders.end(), folders.begin(), folders.end());
			continuationToken = outcome.GetResult().GetNextContinuationToken();
		}
	} while (!continuationToken.empty());
	//std::cout << allFolders.size() << " object(s) found:" << std::endl;

	for (const auto &folder: allFolders) {
		folderResults.push_back(commonPrefixToFolderName(folder.GetPrefix()));
		//std::cout << "  " << folderResults.back() << std::endl;
	}

	return true;
}


bool TuringS3Client::deleteBucket(const Aws::String& bucketName) {
	Aws::S3Crt::Model::DeleteBucketRequest request;
	request.SetBucket(bucketName);

	Aws::S3Crt::Model::DeleteBucketOutcome outcome = _client->DeleteBucket(request);
	if (!outcome.IsSuccess()) {
		const auto& err = outcome.GetError();
		std::cerr << "Error: deleteBucket: " <<
			err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
	} else {
		std::cout << "Deleted bucket " << bucketName <<
			" in the specified AWS Region." << std::endl;
	}

	return outcome.IsSuccess();
}

Result<void> TuringS3Client::uploadFile(const Aws::String& fileName,const Aws::String& bucketName, const Aws::String& keyName){
	Aws::S3Crt::Model::PutObjectRequest request;
	request.WithBucket(bucketName).WithKey(keyName).SetIfNoneMatch("*");
	std::shared_ptr<Aws::IOStream> inputData= Aws::MakeShared<Aws::FStream>("TuringS3Client", fileName.c_str(), std::ios_base::in | std::ios_base::binary);

	if (!*inputData) {
		std::cerr << "Error unable to read file " << fileName << std::endl;
		return Error::result(ErrorType::CANNOT_OPEN_FILE);
	}

	request.SetBody(inputData);

	Aws::S3Crt::Model::PutObjectOutcome outcome = _client->PutObject(request);

	if (!outcome.IsSuccess()) {
		std::cerr << "Error: uploadFile: " <<
			outcome.GetError().GetMessage() << std::endl;

		switch (outcome.GetError().GetErrorType()) {
			case Aws::S3Crt::S3CrtErrors::NO_SUCH_KEY:
				return Error::result(ErrorType::INVALID_KEY_NAME);
			case Aws::S3Crt::S3CrtErrors::NO_SUCH_BUCKET:
				return Error::result(ErrorType::INVALID_BUCKET_NAME);
			default:
				return Error::result(ErrorType::CANNOT_UPLOAD_FILE);
		}
	} 

	//std::cout << "Added File'" << fileName << "' to bucket '"
		//<< bucketName << "'.";

	return {};

}

Result<void> TuringS3Client::downloadFile(const Aws::String& fileName,const Aws::String& bucketName, const Aws::String& keyName){
	Aws::S3Crt::Model::GetObjectRequest request;
	request.WithBucket(bucketName).WithKey(keyName);
	std::ofstream outputFileStream(fileName.c_str(), std::ios_base::out | std::ios_base::binary);

	if (!outputFileStream.is_open()) {
		std::cerr << "Error unable to write to file " << fileName << std::endl;
		return Error::result(ErrorType::CANNOT_OPEN_FILE);
	}

	Aws::S3Crt::Model::GetObjectOutcome outcome = _client->GetObject(request);

	if (!outcome.IsSuccess()) {
		std::cerr << "Error: downloadFile: " <<
			outcome.GetError().GetMessage() << std::endl;

		switch (outcome.GetError().GetErrorType()) {
			case Aws::S3Crt::S3CrtErrors::NO_SUCH_KEY:
				return Error::result(ErrorType::INVALID_KEY_NAME);
			case Aws::S3Crt::S3CrtErrors::NO_SUCH_BUCKET:
				return Error::result(ErrorType::INVALID_BUCKET_NAME);
			default:
				return Error::result(ErrorType::CANNOT_DOWNLOAD_FILE);
		}
	}
	outputFileStream << outcome.GetResult().GetBody().rdbuf();
	outputFileStream.flush();
	outputFileStream.close();
	//std::cout << "Downloaded File'" << fileName << "' from bucket '"
		//<< bucketName << "'."<<std::endl;

	return {};
}

//No trailing forward slash at the end of the prefix parameter!
Result<void> TuringS3Client::uploadDirectory(const std::string& directory,const std::string& bucketName, const std::string& prefix){
	spdlog::info("entering dir");
	bool visitorResult = false;  
	auto visitor = [&prefix,&bucketName, this,&visitorResult](const Aws::FileSystem::DirectoryTree*, const Aws::FileSystem::DirectoryEntry& entry)
		{
			if (entry && entry.fileType == Aws::FileSystem::FileType::File)
			{
				//Change to fmt
				Aws::StringStream ssKey;
				Aws::String relativePath = entry.relativePath;

				ssKey << prefix << "/" << relativePath;
				Aws::String keyName = ssKey.str();
				std::cout<<"Uploading: "<<keyName<<std::endl;
				if(auto res = uploadFile(entry.path, bucketName, keyName); !res ){
					visitorResult = false;
					return false;
				}
			}

			visitorResult = true;
			return true;
		};

	Aws::FileSystem::DirectoryTree dir(directory);
	dir.TraverseDepthFirst(visitor);
	if(!visitorResult){
		return Error::result(ErrorType::CANNOT_UPLOAD_DIRECTORY);
	}

	return {};
}

bool isFolder(const std::string& path){
	return (path.find_last_of('/') == path.size() - 1);
}

Result<void> TuringS3Client::downloadDirectory(const std::string& directory,const std::string& bucketName, const std::string& prefix){
	//Add error handling
	std::filesystem::create_directory(directory);//Add error

	Aws::String downloadPath;
	std::vector<std::string> results; 

	listKeys(bucketName, results, prefix);
	if(results.empty()){
		return Error::result(ErrorType::INVALID_DIRECTORY_NAME);
	}

	for(const auto& result: results){
		std::cout<<"key name is:"<<result<<std::endl;
		if(!isFolder(result)){
			auto pos = result.find(prefix) + prefix.size();
			downloadPath = fmt::format("{}/{}", directory, result.substr(pos));

			auto lastDelimiter = downloadPath.find_last_of('/');
			std::filesystem::create_directories(downloadPath.substr(0,lastDelimiter).c_str());//Add error handling
			if(auto res = downloadFile(downloadPath, bucketName, result); !res){
				return Error::result(ErrorType::CANNOT_DOWNLOAD_DIRECTORY);
			}
		}
	}

	return {};
}

}
