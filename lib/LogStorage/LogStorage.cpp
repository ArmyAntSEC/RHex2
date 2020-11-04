#include "LogStorage.h"
#include <Arduino.h>
#include <stdlib.h>

LogStorage Log;

LogStorage::LogStorage():
    bufferSize(LOG_BUFFER_LEN), writePos(0), looped(false)
{
    memset( this->buffer, 0, this->bufferSize*sizeof(char) );
}

void LogStorage::writeToBuffer( char const* buffer )
{
    //Serial.print ( buffer );
    const int bufferLen = strlen(buffer);
    if ( bufferLen + this->writePos < (this->bufferSize-1) ) 
    {
        strcpy( this->buffer+this->writePos, buffer );
        this->writePos += bufferLen;
    } else {
        int spaceLeft = (this->bufferSize-1) - this->writePos;
        strncpy( this->buffer+this->writePos, buffer, spaceLeft );

        int overFlowLength = bufferLen - spaceLeft;
        strcpy( this->buffer, buffer + spaceLeft );

        this->writePos = overFlowLength;
        
        this->looped = true;
    }   
}

LogStorage& LogStorage::operator<< ( char const* str )
{
    this->writeToBuffer( str );
    return *this;
}

LogStorage& LogStorage::operator<< ( String str )
{
    this->writeToBuffer( str.c_str() );
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
    Serial.println( "**** DUMPING LOG ***** " );
    if ( looped ) {
        //We write out what remains of the olf buffer.
        Serial.println( this->buffer + this->writePos + 1 );
    }
    Serial.println ( this->buffer );
}