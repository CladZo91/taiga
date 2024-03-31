#pragma once

#include <string>

// Forward declarations if necessary
namespace taiga {
class Settings;
}

namespace track {

class QbittorrentAPI {
public:
  QbittorrentAPI(); //Default Constructor
  ~QbittorrentAPI();  // Default Constructor
  bool Authenticate();
  bool IsAuthenticated() const;
  bool SendMagnetLink(const std::string& magnet_link);

private:
  std::string api_address_;
  std::string api_username_;
  std::string api_password_;
  std::string api_category_;
  std::string session_cookie_;

  bool CheckAuthentication();
  void StoreSessionCookie(const std::string& cookie);
  bool IsSessionCookieExpired() const;
};

}  // namespace track
