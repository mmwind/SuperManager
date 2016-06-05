/*
 * crypt.cpp
 *
 *  Created on: 5 июня 2016 г.
 *      Author: lladmin
 */
#include <crypt.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

std::string getHash(std::string inputStr){
	  CryptoPP::SHA256 hash;
	  byte digest[CryptoPP::SHA256::DIGESTSIZE];

	  hash.Update( (const byte*) inputStr.c_str(), inputStr.length());
	  hash.Final(digest);

	  CryptoPP::HexEncoder encoder;
	  std::string output;

	  encoder.Attach( new CryptoPP::StringSink( output ) );
	  encoder.Put( digest, sizeof(digest) );
	  encoder.MessageEnd();
	  return(output);
}

