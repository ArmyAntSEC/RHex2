#include "LogStorage.h"
#include <Arduino.h>
#include <stdlib.h>

LogStorage Log;

LogStorage::LogStorage():
    bufferSize(LOG_BUFFER_LEN), writePos(0)
{
    memset( this->buffer, 0, this->bufferSize*sizeof(char) );
}

void LogStorage::writeToBuffer( char const* buffer )
{
    const int bufferLen = strlen(buffer);
    if ( bufferLen + this->writePos < this->bufferSize ) 
    {
        strcpy( this->buffer+this->writePos, buffer );
        this->writePos += bufferLen;
    } else {
        //We write a message saying we have overflowed
        char temp[] = "\n**OVERFLOW**\n";
        int tempLen = strlen( temp );
        strcpy( this->buffer+this->bufferSize-tempLen, temp );
        
        //Make sure we do not overwrite anything allready in the log.
        this->writePos = this->bufferSize;
    }   
}

LogStorage& LogStorage::operator<< ( char const* str )
{
    this->writeToBuffer( str );
    return *this;
}

LogStorage& LogStorage::operator<< ( long int num )
{
    char buffer[20];
    sprintf ( buffer, "%ld", num);
    
    this->writeToBuffer( buffer );

    return *this;
}

LogStorage& LogStorage::operator<< ( int num )
{
    char buffer[20];
    sprintf ( buffer, "%d", num);
    
    this->writeToBuffer( buffer );

    return *this;    
}

LogStorage& LogStorage::operator<< ( long unsigned int num )
{
    char buffer[20];
    sprintf ( buffer, "%ld", num);
    
    this->writeToBuffer( buffer );

    return *this;
}

LogStorage& LogStorage::operator<< ( unsigned int num )
{
    char buffer[20];
    sprintf ( buffer, "%u", num);
    
    this->writeToBuffer( buffer );

    return *this;    
}

LogStorage& LogStorage::operator<< ( double num )
{
    char buffer[20];
    sprintf ( buffer, "%f", num);
    
    this->writeToBuffer( buffer );

    return *this;
}

LogStorage& LogStorage::operator<< (  _EndlCode )
{
    char const * buffer = "\n";
    
    this->writeToBuffer( buffer );

    //Serial.print( this->buffer );
    
    return *this;   
}

void LogStorage::sendToSerial()
{
    Serial.println ( this->buffer );
}