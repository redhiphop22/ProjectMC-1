// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace protocol
{

using global::System;
using global::System.Collections.Generic;
using global::FlatBuffers;

public struct SERVER_CONNECT_S2C : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static SERVER_CONNECT_S2C GetRootAsSERVER_CONNECT_S2C(ByteBuffer _bb) { return GetRootAsSERVER_CONNECT_S2C(_bb, new SERVER_CONNECT_S2C()); }
  public static SERVER_CONNECT_S2C GetRootAsSERVER_CONNECT_S2C(ByteBuffer _bb, SERVER_CONNECT_S2C obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public SERVER_CONNECT_S2C __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public common.RESULT_CODE Result { get { int o = __p.__offset(4); return o != 0 ? (common.RESULT_CODE)__p.bb.GetUint(o + __p.bb_pos) : common.RESULT_CODE.ERROR_FAIL; } }
  public ulong Uid { get { int o = __p.__offset(6); return o != 0 ? __p.bb.GetUlong(o + __p.bb_pos) : (ulong)0; } }
  public common.ACCOUNT_AUTHORITY Authority { get { int o = __p.__offset(8); return o != 0 ? (common.ACCOUNT_AUTHORITY)__p.bb.Get(o + __p.bb_pos) : common.ACCOUNT_AUTHORITY.GUEST; } }
  public common.ACCOUNT_BLOCK_TYPE BlockType { get { int o = __p.__offset(10); return o != 0 ? (common.ACCOUNT_BLOCK_TYPE)__p.bb.Get(o + __p.bb_pos) : common.ACCOUNT_BLOCK_TYPE.NONE; } }
  public ulong BlockDate { get { int o = __p.__offset(12); return o != 0 ? __p.bb.GetUlong(o + __p.bb_pos) : (ulong)0; } }

  public static Offset<protocol.SERVER_CONNECT_S2C> CreateSERVER_CONNECT_S2C(FlatBufferBuilder builder,
      common.RESULT_CODE result = common.RESULT_CODE.ERROR_FAIL,
      ulong uid = 0,
      common.ACCOUNT_AUTHORITY authority = common.ACCOUNT_AUTHORITY.GUEST,
      common.ACCOUNT_BLOCK_TYPE block_type = common.ACCOUNT_BLOCK_TYPE.NONE,
      ulong block_date = 0) {
    builder.StartTable(5);
    SERVER_CONNECT_S2C.AddBlockDate(builder, block_date);
    SERVER_CONNECT_S2C.AddUid(builder, uid);
    SERVER_CONNECT_S2C.AddResult(builder, result);
    SERVER_CONNECT_S2C.AddBlockType(builder, block_type);
    SERVER_CONNECT_S2C.AddAuthority(builder, authority);
    return SERVER_CONNECT_S2C.EndSERVER_CONNECT_S2C(builder);
  }

  public static void StartSERVER_CONNECT_S2C(FlatBufferBuilder builder) { builder.StartTable(5); }
  public static void AddResult(FlatBufferBuilder builder, common.RESULT_CODE result) { builder.AddUint(0, (uint)result, 0); }
  public static void AddUid(FlatBufferBuilder builder, ulong uid) { builder.AddUlong(1, uid, 0); }
  public static void AddAuthority(FlatBufferBuilder builder, common.ACCOUNT_AUTHORITY authority) { builder.AddByte(2, (byte)authority, 0); }
  public static void AddBlockType(FlatBufferBuilder builder, common.ACCOUNT_BLOCK_TYPE blockType) { builder.AddByte(3, (byte)blockType, 0); }
  public static void AddBlockDate(FlatBufferBuilder builder, ulong blockDate) { builder.AddUlong(4, blockDate, 0); }
  public static Offset<protocol.SERVER_CONNECT_S2C> EndSERVER_CONNECT_S2C(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<protocol.SERVER_CONNECT_S2C>(o);
  }
};


}
