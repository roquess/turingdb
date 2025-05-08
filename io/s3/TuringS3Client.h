#pragma once

#include "S3Result.h"
#include <memory>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/s3-crt/ClientConfiguration.h>

namespace Aws {
    struct SDKOptions;
    namespace S3 {
        class S3Client;
        class S3ClientConfiguration;
    }
    namespace S3Crt {
	class S3CrtClient;
    }
    namespace Transfer {
	class TransferManager;

    }
    namespace Utils {
	namespace Threading {
	    class PooledThreadExecutor;
	}
    }
}

namespace S3 {

class TuringS3Client{
public:
	TuringS3Client();
	~TuringS3Client();

	void init();

	void listBuckets();
	bool createBucket(const std::string& bucketName);
	bool deleteBucket(const std::string& bucketName);

	bool listKeys(const std::string& bucketName, std::vector<std::string>& keyResults, const std::string& prefix);
	bool listFiles(const std::string& bucketName, std::vector<std::string>& keyResults, const std::string& prefix);
	bool listFolders(const std::string& bucketName, std::vector<std::string>& folderResults, const std::string& prefix);
	
	Result<void> uploadFile(const std::string& fileName,const std::string& bucketName, const std::string& keyName);
	Result<void> downloadFile(const std::string& fileName,const std::string& bucketName, const std::string& keyName);

	Result<void> uploadDirectory(const std::string& directory,const std::string& bucketName, const std::string& prefix);
	Result<void> downloadDirectory(const std::string& directory,const std::string& bucketName, const std::string& prefix);

	//mutable struct with many options.
	std::unique_ptr<Aws::S3Crt::ClientConfiguration> clientConfig;
private:
	std::unique_ptr<Aws::SDKOptions> _options ;
	std::shared_ptr<Aws::S3Crt::S3CrtClient> _client {nullptr};
	std::shared_ptr<Aws::Transfer::TransferManager> _transferManager {nullptr};
	std::shared_ptr<Aws::Utils::Threading::PooledThreadExecutor> _executor {nullptr};

};



}
