#include "qbittorrentapi.h"
#include "track/qbittorrentapi.h"

#include "taiga/http.h"  // Ensure the correct path to your HTTP classes
#include "taiga/settings_keys.h"  // For accessing the settings values

// Define any constants needed for the API calls
namespace {
const char* kApiPathLogin = "/api/v2/auth/login";
const char* kApiPathAddTorrent = "/api/v2/torrents/add";
}  // namespace


namespace track {

  // Implement the constructor
  QbittorrentAPI::QbittorrentAPI() {
    // Initialize members, if any
  }

  // Implement the destructor
  QbittorrentAPI::~QbittorrentAPI() {
    // Clean up, if needed
  }

  // Implement the Authenticate method
  bool QbittorrentAPI::Authenticate() {
    taiga::http::Request request;
    request.set_target(taiga::settings.GetQbittorrentApiAddress() +
                       kApiPathLogin);
    request.set_method(taiga::http::Method::Post);
    request.add_body_param("username",
                           taiga::settings.GetQbittorrentApiUsername());
    request.add_body_param("password",
                           taiga::settings.GetQbittorrentApiPassword());

    // Send the request and handle the response
    taiga::http::Response response = taiga::http::Send(request);
    if (response.status_code() == 200) {
      session_cookie_ =
          response.cookie("SID");  // Example, actual cookie name may differ
      return true;
    }
    return false;
  }

    // Implement the IsAuthenticated method
  bool QbittorrentAPI::IsAuthenticated() const {
    return !session_cookie_.empty();
  }

  // Implement the SendMagnetLink method
  bool QbittorrentAPI::SendMagnetLink(const std::string& magnet_link) {
    if (!IsAuthenticated() && !Authenticate()) {
      return false;  // Authentication failed
    }

    taiga::http::Request request;
    request.set_target(taiga::settings.GetQbittorrentApiAddress() +
                       kApiPathAddTorrent);
    request.set_method(taiga::http::Method::Post);
    request.add_header("Cookie", "SID=" + session_cookie_);
    request.add_body_param("urls", magnet_link);
    request.add_body_param("category",
                           taiga::settings.GetQbittorrentApiCategory());

    // If additional torrent options are required, they would be set here

    taiga::http::Response response = taiga::http::Send(request);
    if (response.status_code() == 200) {
      // The torrent was added successfully
      return true;
    } else {
      // Handle different HTTP errors accordingly
      return false;
    }
  }

}  // namespace track
// Implement other necessary methods or functionality...
