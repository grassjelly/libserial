#ifndef _SerialPort_h_
#    include <SerialPort.h>
#endif

#ifndef _termios_h_INCLUDED_
#    include <termios.h>
#    define _termios_h_INCLUDED_
#endif

#ifndef _fcntl_h_INCLUDED_
#    include <fcntl.h>
#    define _fcntl_h_INCLUDED_
#endif

#ifndef _unistd_h_INCLUDED_
#    include <unistd.h>
#    define _unistd_h_INCLUDED_
#endif

#ifndef _sys__ioctl_h_INCLUDED_
#    include <sys/ioctl.h>
#    define _sys__ioctl_h_INCLUDED_
#endif

#ifndef _std_cerrno_INCLUDED_
#    include <cerrno>
#    define _std_cerrno_INCLUDED_
#endif


namespace {
    //
    // Various error messages used in this file while throwing
    // exceptions.
    //
    const std::string ERR_MSG_PORT_NOT_OPEN     = "Serial port not open." ;
    const std::string ERR_MSG_PORT_ALREADY_OPEN = "Serial port already open." ;
    const std::string ERR_MSG_UNSUPPORTED_BAUD  = "Unsupported baud rate." ;
    const std::string ERR_MSG_UNKNOWN_BAUD      = "Unknown baud rate." ;
    const std::string ERR_MSG_INVALID_PARITY    = "Invalid parity setting." ;
    const std::string ERR_MSG_INVALID_STOP_BITS = "Invalid number of stop bits." ;
    const std::string ERR_MSG_INVALID_FLOW_CONTROL = "Invalid flow control." ;
} ;

class SerialPortImpl {
public:
    /**
     * Constructor.
     */
    SerialPortImpl( const std::string& serialPortName ) ;

    /**
     * Destructor.
     */
    ~SerialPortImpl() ;

    /**
     * Open the serial port. 
     */
    void Open() 
        throw( SerialPort::OpenFailed,
               SerialPort::AlreadyOpen ) ;

    /**
     * Check if the serial port is currently open.
     */
    bool 
    IsOpen() const ;

    /**
     * Close the serial port. 
     */
    void
    Close() 
        throw(SerialPort::NotOpen) ;

    /**
     * Set the baud rate of the serial port.
     */
    void
    SetBaudRate( const SerialPort::BaudRate baudRate )
        throw( SerialPort::NotOpen, 
               SerialPort::UnsupportedBaudRate,
               std::invalid_argument, 
               std::runtime_error ) ;

    /**
     * Get the current baud rate. 
     */
    SerialPort::BaudRate
    GetBaudRate() const 
        throw( SerialPort::NotOpen,
               std::runtime_error ) ;

    /**
     * Set the character size.
     */
    void
    SetCharSize( const SerialPort::CharacterSize charSize )
        throw( SerialPort::NotOpen, 
               std::invalid_argument,
               std::runtime_error ) ;

    /**
     * Get the current character size.
     */
    SerialPort::CharacterSize
    GetCharSize() const
        throw( SerialPort::NotOpen,
               std::runtime_error )  ;

    void
    SetParity( const SerialPort::Parity parityType )
        throw( SerialPort::NotOpen,
               std::invalid_argument, 
               std::runtime_error )  ;

    SerialPort::Parity
    GetParity() const 
        throw(SerialPort::NotOpen) ;

    void
    SetNumOfStopBits( const SerialPort::StopBits numOfStopBits ) 
        throw( SerialPort::NotOpen,
               std::invalid_argument )  ;

    SerialPort::StopBits
    GetNumOfStopBits() const 
        throw(SerialPort::NotOpen) ;

    void
    SetFlowControl( const SerialPort::FlowControl flowControl ) 
        throw( SerialPort::NotOpen,
               std::invalid_argument ) ;

    SerialPort::FlowControl
    GetFlowControl() const 
        throw( SerialPort::NotOpen ) ;

    bool
    IsDataAvailable() const 
        throw( SerialPort::NotOpen,
               std::runtime_error ) ;

    unsigned char
    ReadByte()
        throw( SerialPort::NotOpen,
               std::runtime_error ) ;

    void
    WriteByte( const unsigned char dataByte )
        throw( SerialPort::NotOpen,
               std::runtime_error ) ;
private:
    /**
     * Name of the serial port. On POSIX systems this is the name of
     * the device file.
     */
    std::string mSerialPortName ;

    /**
     * Flag that indicates whether the serial port is currently open.
     */
    bool mIsOpen ;
    
    /**
     * The file descriptor corresponding to the serial port.
     */
    int mFileDescriptor ;

    /**
     * Serial port settings are saved into this variable immediately
     * after the port is opened. These settings are restored when the
     * serial port is closed.
     */
    termios mOldPortSettings ;
} ;

SerialPort::SerialPort( const std::string& serialPortName ) :
    mSerialPortImpl(new SerialPortImpl(serialPortName) ) 
{
    /* empty */
}

SerialPort::~SerialPort() 
    throw()
{
    /*
     * Close the serial port if it is open.
     */
    if ( this->IsOpen() ) {
        this->Close() ;
    }
    /*
     * Free the memory allocated to the implementation instance.
     */
    if ( mSerialPortImpl ) {
        delete mSerialPortImpl ;
    }
    return ;
}

void
SerialPort::Open( const BaudRate      baudRate,
                  const CharacterSize charSize,
                  const Parity        parityType,
                  const StopBits      stopBits,
                  const FlowControl   flowControl )
    throw( OpenFailed,
           AlreadyOpen,
           UnsupportedBaudRate,
           std::invalid_argument ) 
{
    //
    // Open the serial port. 
    mSerialPortImpl->Open() ;
    //
    // Set the various parameters of the serial port if it is open.
    this->SetCharSize(charSize) ;
    this->SetParity(parityType) ;
    this->SetNumOfStopBits(stopBits) ;
    this->SetFlowControl( flowControl ) ;
    //
    // All done.
    return ;
}

bool
SerialPort::IsOpen() const
{
    return mSerialPortImpl->IsOpen() ;
}

void
SerialPort::Close()
    throw(NotOpen) 
{
    mSerialPortImpl->Close() ;
    return ;
}

void
SerialPort::SetBaudRate( const BaudRate baudRate )
    throw( UnsupportedBaudRate,
           NotOpen, 
           std::invalid_argument ) 
{
    mSerialPortImpl->SetBaudRate( baudRate ) ;
    return ;
}

SerialPort::BaudRate
SerialPort::GetBaudRate() const 
    throw( NotOpen,
           std::runtime_error )
{
    return mSerialPortImpl->GetBaudRate() ;
}


void
SerialPort::SetCharSize( const CharacterSize charSize )
    throw( NotOpen, 
           std::invalid_argument ) 
{
    mSerialPortImpl->SetCharSize(charSize) ;
}

SerialPort::CharacterSize
SerialPort::GetCharSize() const 
    throw(NotOpen) 
{
    return mSerialPortImpl->GetCharSize() ;
}

void
SerialPort::SetParity( const Parity parityType ) 
    throw( NotOpen,
           std::invalid_argument ) 
{
    mSerialPortImpl->SetParity( parityType ) ;
    return ;
}

SerialPort::Parity
SerialPort::GetParity() const 
    throw(NotOpen) 
{
    return mSerialPortImpl->GetParity() ;
}

void
SerialPort::SetNumOfStopBits( const StopBits numOfStopBits ) 
    throw( NotOpen,
           std::invalid_argument ) 
{
    mSerialPortImpl->SetNumOfStopBits(numOfStopBits) ;
    return ;
}

SerialPort::StopBits
SerialPort::GetNumOfStopBits() const 
    throw(NotOpen) 
{
    return mSerialPortImpl->GetNumOfStopBits() ;
}


void
SerialPort::SetFlowControl( const FlowControl   flowControl ) 
    throw( NotOpen,
           std::invalid_argument ) 
{
    mSerialPortImpl->SetFlowControl( flowControl ) ;
    return ;
}

SerialPort::FlowControl
SerialPort::GetFlowControl() const 
    throw( NotOpen ) 
{
    return mSerialPortImpl->GetFlowControl() ;
}

bool 
SerialPort::IsDataAvailable() const
    throw(NotOpen) 
{
    return mSerialPortImpl->IsDataAvailable() ;
}

unsigned char
SerialPort::ReadByte()
    throw( SerialPort::NotOpen,
           std::runtime_error )
{
    return mSerialPortImpl->ReadByte() ;
}

void
SerialPort::WriteByte( const unsigned char dataByte )
    throw( SerialPort::NotOpen,
           std::runtime_error )
{
    mSerialPortImpl->WriteByte( dataByte ) ;
    return ;
} 
/* ------------------------------------------------------------ */
inline
SerialPortImpl::SerialPortImpl( const std::string& serialPortName ) :
    mSerialPortName(serialPortName),
    mIsOpen(false),
    mFileDescriptor(-1), 
    mOldPortSettings()
{
    /* empty */
}

inline
SerialPortImpl::~SerialPortImpl()
{
    //
    // Close the serial port if it is open.
    //
    if ( this->IsOpen() ) {
        this->Close() ;
    }
    return ;
}

inline
void
SerialPortImpl::Open()
    throw( SerialPort::OpenFailed,
           SerialPort::AlreadyOpen ) 
{
    /*
     * Throw an exception if the port is already open.
     */
    if ( this->IsOpen() ) {
        throw SerialPort::AlreadyOpen( ERR_MSG_PORT_ALREADY_OPEN ) ;
    }
    /*
     * Try to open the serial port and throw an exception if we are
     * not able to open it.
     */
    mFileDescriptor = open( mSerialPortName.c_str(), 
                            O_RDWR | O_NOCTTY | O_NONBLOCK ) ;
    if ( mFileDescriptor < 0 ) {
        throw SerialPort::OpenFailed( strerror(errno) )  ;
    }
    /*
     * Save the current settings of the serial port so they can be
     * restored when the serial port is closed.
     */
    if ( tcgetattr( mFileDescriptor, 
                    &mOldPortSettings ) < 0 ) {
        throw SerialPort::OpenFailed( strerror(errno) ) ;
    }

    //
    // Copy the old settings and modify them as necessary.
    //
    termios port_settings = mOldPortSettings ;
    
    //
    // Zero out all local and output flags. 
    //
    port_settings.c_lflag = 0 ;
    port_settings.c_oflag = 0 ;
    
    //
    // Enable the receiver (CREAD) and ignore modem control lines
    // (CLOCAL).
    //
    port_settings.c_cflag |= CREAD | CLOCAL ;

    //
    // Set the VMIN and VTIME parameters to zero by default. VMIN is
    // the minimum number of characters for non-canonical read and
    // VTIME is the timeout in deciseconds for non-canonical
    // read. Setting both of these parameters to zero implies that a
    // read will return immediately only giving the currently
    // available characters.
    //
    port_settings.c_cc[ VMIN  ] = 0 ;
    port_settings.c_cc[ VTIME ] = 0 ;
    
    /*
     * Write the new settings to the port.
     */
    if ( tcsetattr( mFileDescriptor, 
                    TCSANOW,
                    &port_settings ) < 0 ) {
        throw SerialPort::OpenFailed( strerror(errno) ) ;
    }

    /*
     * Direct all SIGIO and SIGURG signals for the port to the current
     * process.
     */
    if ( fcntl( mFileDescriptor, 
                F_SETOWN, 
                getpid() ) < 0 ) {
        throw SerialPort::OpenFailed( strerror(errno) ) ;
    }

    /*
     * The serial port is open at this point.
     */
    mIsOpen = true ;
    return ;
}

inline
bool
SerialPortImpl::IsOpen() const
{
    return mIsOpen ;
}

inline
void
SerialPortImpl::Close() 
    throw( SerialPort::NotOpen )
{
    //
    // Throw an exception if the serial port is not open.
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Restore the old settings of the port. 
    //
    tcsetattr( mFileDescriptor, 
               TCSANOW, 
               &mOldPortSettings ) ;
    //
    // Close the serial port file descriptor. 
    //
    close(mFileDescriptor) ;
    //
    // The port is not open anymore.
    //
    mIsOpen = false ;
    //
    return ;
}

inline
void
SerialPortImpl::SetBaudRate( const SerialPort::BaudRate baudRate )
    throw( SerialPort::NotOpen, 
           SerialPort::UnsupportedBaudRate,
           std::invalid_argument,
           std::runtime_error ) 
{
    //
    // Throw an exception if the serial port is not open.
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Get the current settings of the serial port.
    //
    termios port_settings ;
    if ( tcgetattr( mFileDescriptor, 
                    &port_settings ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    //
    // Set the baud rate for both input and output.
    //
    if ( ( cfsetispeed( &port_settings, 
                        baudRate ) < 0 ) ||
         ( cfsetospeed( &port_settings, 
                        baudRate ) < 0 ) ) {
        //
        // If any of the settings fail, we abandon this method.
        //
        throw SerialPort::UnsupportedBaudRate( ERR_MSG_UNSUPPORTED_BAUD ) ;
    }
    //
    // Set the new attributes of the serial port. 
    //
    if ( tcsetattr( mFileDescriptor, 
                    TCSANOW, 
                    &port_settings ) < 0 ) {
        throw SerialPort::UnsupportedBaudRate( strerror(errno) ) ;
    }
    return ;
}

SerialPort::BaudRate
SerialPortImpl::GetBaudRate() const
    throw( SerialPort::NotOpen, 
           std::runtime_error )
{
    //
    // Make sure that the serial port is open.
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Read the current serial port settings. 
    //
    termios current_settings ;
    if ( tcgetattr( mFileDescriptor, 
                    &current_settings ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    //
    // Obtain the input baud rate from the current settings.
    //
    return SerialPort::BaudRate(cfgetispeed( &current_settings )) ;
}

void
SerialPortImpl::SetCharSize( const SerialPort::CharacterSize charSize )
    throw( SerialPort::NotOpen, 
           std::invalid_argument,
           std::runtime_error ) 
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Get the current settings of the serial port.
    //
    termios port_settings ;
    if ( tcgetattr( mFileDescriptor, 
                    &port_settings ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    //
    // Set the character size. 
    //
    port_settings.c_cflag &= ~CSIZE ;
    port_settings.c_cflag |= charSize ;
    //
    // Apply the modified settings. 
    //
    if ( tcsetattr( mFileDescriptor, 
                    TCSANOW, 
                    &port_settings ) < 0 ) {
        throw std::invalid_argument( strerror(errno) ) ;
    }
    return ;
}

SerialPort::CharacterSize
SerialPortImpl::GetCharSize() const 
    throw( SerialPort::NotOpen, 
           std::runtime_error ) 
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Get the current port settings. 
    //
    termios port_settings ;
    if ( tcgetattr( mFileDescriptor, 
                    &port_settings ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    //
    // Read the character size from the setttings. 
    //
    return SerialPort::CharacterSize( port_settings.c_cflag & CSIZE ) ;
}

void
SerialPortImpl::SetParity( const SerialPort::Parity parityType )
    throw( SerialPort::NotOpen,
           std::invalid_argument, 
           std::runtime_error ) 
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Get the current port settings. 
    //
    termios port_settings ;
    if ( tcgetattr( mFileDescriptor, 
                    &port_settings ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    //
    // Set the parity type depending on the specified parameter. 
    //
    switch( parityType ) {
    case SerialPort::PARITY_EVEN:
        port_settings.c_cflag |= PARENB ;
        port_settings.c_cflag &= ~PARODD ;
        break ;
    case SerialPort::PARITY_ODD:
        port_settings.c_cflag |= ( PARENB | PARODD ) ;
        break ;
    case SerialPort::PARITY_NONE:
        port_settings.c_cflag &= ~(PARENB) ;
        break ;
    default:
        throw std::invalid_argument( ERR_MSG_INVALID_PARITY ) ;
        break ;
    }
    //
    // Apply the modified port settings. 
    //
    if ( tcsetattr( mFileDescriptor, 
                    TCSANOW, 
                    &port_settings ) < 0 ) {
        throw std::invalid_argument( strerror(errno) ) ;
    }
    return ;
}

SerialPort::Parity
SerialPortImpl::GetParity() const 
    throw(SerialPort::NotOpen) 
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Get the current port settings. 
    //
    termios port_settings ;
    if ( tcgetattr( mFileDescriptor, 
                    &port_settings ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    //
    // Get the parity type from the current settings.
    //
    if ( port_settings.c_cflag & PARENB ) {
        //
        // Parity is enabled. Lets check if it is odd or even. 
        //
        if ( port_settings.c_cflag & PARODD ) {
            return SerialPort::PARITY_ODD ;
        } else {
            return SerialPort::PARITY_EVEN ;
        }
    } 
    //
    // Parity is disabled.
    //
    return SerialPort::PARITY_NONE ;
}

void
SerialPortImpl::SetNumOfStopBits( const SerialPort::StopBits numOfStopBits ) 
    throw( SerialPort::NotOpen,
           std::invalid_argument ) 
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Get the current port settings. 
    //
    termios port_settings ;
    if ( tcgetattr( mFileDescriptor, 
                    &port_settings ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    //
    // Set the number of stop bits. 
    //
    switch( numOfStopBits ) {
    case SerialPort::STOP_BITS_1:
        port_settings.c_cflag &= ~(CSTOPB) ;
        break ;
    case SerialPort::STOP_BITS_2:
        port_settings.c_cflag |= CSTOPB ;
        break ;        
    default:
        throw std::invalid_argument( ERR_MSG_INVALID_STOP_BITS ) ;
        break ;
    } 
    //
    // Apply the modified settings. 
    //
    if ( tcsetattr( mFileDescriptor, 
                    TCSANOW, 
                    &port_settings ) < 0 ) {
        throw std::invalid_argument( strerror(errno) ) ;
    }
    return ;
}

SerialPort::StopBits
SerialPortImpl::GetNumOfStopBits() const 
    throw(SerialPort::NotOpen) 
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Get the current port settings. 
    //
    termios port_settings ;
    if ( tcgetattr( mFileDescriptor, 
                    &port_settings ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    //
    // If CSTOPB is set then we are using two stop bits, otherwise we
    // are using 1 stop bit.
    //
    if ( port_settings.c_cflag & CSTOPB ) {
        return SerialPort::STOP_BITS_2 ;
    }
    return SerialPort::STOP_BITS_1 ;
}

void
SerialPortImpl::SetFlowControl( const SerialPort::FlowControl   flowControl ) 
    throw( SerialPort::NotOpen,
           std::invalid_argument ) 
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Get the current port settings. 
    //
    termios port_settings ;
    if ( tcgetattr( mFileDescriptor, 
                    &port_settings ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    //
    // Set the flow control.
    //
    switch( flowControl ) {
    case SerialPort::FLOW_CONTROL_HARD:
        port_settings.c_cflag |= CRTSCTS ;
        break ;
    case SerialPort::FLOW_CONTROL_NONE:
        port_settings.c_cflag &= ~(CRTSCTS) ;
        break ;
    default:
        throw std::invalid_argument( ERR_MSG_INVALID_FLOW_CONTROL ) ;
        break ;
    }
    // 
    // Apply the modified settings. 
    //
    if ( tcsetattr( mFileDescriptor, 
                    TCSANOW, 
                    &port_settings ) < 0 ) {
        throw std::invalid_argument( strerror(errno) ) ;
    }    
    return ;
}

SerialPort::FlowControl
SerialPortImpl::GetFlowControl() const 
    throw( SerialPort::NotOpen ) 
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Get the current port settings. 
    //
    termios port_settings ;
    if ( tcgetattr( mFileDescriptor, 
                    &port_settings ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    //
    // If CRTSCTS is set then we are using hardware flow
    // control. Otherwise, we are not using any flow control.
    //
    if ( port_settings.c_cflag & CRTSCTS ) {
        return SerialPort::FLOW_CONTROL_HARD ;
    }
    return SerialPort::FLOW_CONTROL_NONE ;
}

bool
SerialPortImpl::IsDataAvailable() const 
    throw( SerialPort::NotOpen, 
           std::runtime_error ) 
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Check if any data is available at the serial port. 
    //
    int num_of_bytes_available = 0 ;
    if ( ioctl( mFileDescriptor, 
                FIONREAD, 
                &num_of_bytes_available ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    return (num_of_bytes_available ? true : false) ;
}

unsigned char
SerialPortImpl::ReadByte()
    throw( SerialPort::NotOpen,
           std::runtime_error )
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Wait till data is available.
    //
    while( ! this->IsDataAvailable() ) ;
    //
    // Read one byte of data.
    //
    unsigned char data_byte ; 
    if ( read( mFileDescriptor,
               &data_byte, 
               1 ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    return data_byte ;
}

void
SerialPortImpl::WriteByte( const unsigned char dataByte )
    throw( SerialPort::NotOpen,
           std::runtime_error ) 
{
    //
    // Make sure that the serial port is open. 
    //
    if ( ! this->IsOpen() ) {
        throw SerialPort::NotOpen( ERR_MSG_PORT_NOT_OPEN ) ;
    }
    //
    // Write the data to the serial port.
    //
    if ( write( mFileDescriptor, 
                &dataByte,
                1 ) < 0 ) {
        throw std::runtime_error( strerror(errno) ) ;
    }
    return ;
}