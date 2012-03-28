// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef  PSOCKET_TYPE_H_
#define    PSOCKET_TYPE_H_



namespace  Network{

	class NETWORK_EXPORT PSocketType
	{

	public:
		PSocketType(U32   iType, S8 * inDeli, U32 iDeliSize, U32  iSizeFrom,
			U32 iSizeLen, U32  iSizeCountFrom);

		virtual ~PSocketType(void);

		U32						getSizeFrom()const ;
		U32						getSizeLen() const;
		U32						getSizeCountFrom() const ;
		void					getDelimitation( char *  store, U32  * storeLen) const;
		U32						getBaseHeaderLen( )const;

		U32						getId() const ;

	private:

		//表示包的分隔符，一般没有分隔符，但是http协议的分隔符为："\r\n\r\n"
		char					m_cDelimitation[8];

		//表示包的分隔符的长度，http的分隔符的长度为4个字节。
		U32						m_iDelimitationSize;

		// 表示 表示包长度的那个字段的位置，相对于包头第一个字节的偏移。一般在包的首字节，为0
		U32						m_iSizeFrom;

		// 表示 表示包长度的那个字段占多少个字节的长度。一般为2个字节长度。即此包最大只有2^16字节。
		U32						m_iSizeLen;

		// 表示从哪里计算包的长度，有的是从第0个字节计算，有的是去掉包头开始计算
		U32						m_iSizeCountFrom;

		//此包的类型。
		U32						m_iId;

	};

}

#endif // PSOCKET_TYPE_H_
