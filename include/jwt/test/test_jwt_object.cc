#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <unordered_map>
#include "jwt/jwt.hpp"

void basic_jwt_object_test()
{
  using namespace jwt::params;
  jwt::jwt_object obj(payload({
                                {"a", "b"},
                                {"c", "d"} 
                              }));

  //check with std::map
  std::map<std::string, std::string> m;
  m["a"] = "b";
  m["c"] = "d";

  jwt::jwt_object obj1{payload(m)};
  
  auto obj2 = std::move(obj1);

  std::cout << obj2.payload() << std::endl;

  //check with unordered map of string_view
  std::unordered_map<jwt::string_view, std::string> um = {
    {"a", "b"},
    {"c", "d"}
  };
  jwt::jwt_object obj3{payload(um)};

  obj3.add_claim("f", true)
      .add_claim("time", 176353563)
      .add_claim("exp", std::chrono::system_clock::now())
      ;

  std::cout << jwt::to_json_str(obj3.payload(), true) << std::endl;

  obj3.remove_claim(std::string{"a"});
  std::cout << obj3.payload() << std::endl;

  obj3.secret("secret");
  obj3.header().algo("hs256");

  auto dec_obj = jwt::jwt_decode(obj3.signature(), "secret");
}

void jwt_object_pem_test()
{
  std::string pub_key = 
    R"(-----BEGIN PUBLIC KEY-----
MHYwEAYHKoZIzj0CAQYFK4EEACIDYgAEomxC9ycc8AkXSwWQpu1kN5Fmgy/sD/KJ
qN3tlSZmUEZ3w3c6KYJfK97PMOSZQaUdeydBoq/IOglQQOj8zLqubq5IpaaUiDQ5
0eJg79PvXuLiVUH98cBL/o8sDVB/sGzz
-----END PUBLIC KEY-----)";

  std::string priv_key = 
R"(-----BEGIN EC PRIVATE KEY-----
MIGkAgEBBDBeLCgapjZmvTatMHaYX3A02+0Ys3Tr8kda+E9DFnmCSiCOEig519fT
13edeU8YdDugBwYFK4EEACKhZANiAASibEL3JxzwCRdLBZCm7WQ3kWaDL+wP8omo
3e2VJmZQRnfDdzopgl8r3s8w5JlBpR17J0Gir8g6CVBA6PzMuq5urkilppSINDnR
4mDv0+9e4uJVQf3xwEv+jywNUH+wbPM=
-----END EC PRIVATE KEY-----)";

  jwt::jwt_object obj;
  obj.secret(priv_key);
  obj.header().algo(jwt::algorithm::ES256);

  obj.add_claim("iss", "arun.com")
     .add_claim("where", "airport")
     .add_claim("time_str", "8:18pm 24 Nov 2017")
     .add_claim("id", 1)
     ;

  std::cout << "pem sign " << obj.signature() << std::endl;
}

int main() {
  basic_jwt_object_test();
  jwt_object_pem_test();
  return 0;
}
