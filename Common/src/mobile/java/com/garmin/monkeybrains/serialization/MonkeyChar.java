package com.garmin.monkeybrains.serialization;

import java.nio.ByteBuffer;

/* loaded from: classes.jar:com/garmin/monkeybrains/serialization/MonkeyChar.class */
public class MonkeyChar extends MonkeyType<Integer> {
    private Integer mValue;

    public MonkeyChar(Integer value) {
        super((byte) 19);
        this.mValue = value;
    }

    public MonkeyChar(byte[] bytes) {
        super(bytes[0]);
        ByteBuffer bb = ByteBuffer.wrap(bytes, 1, bytes.length - 1);
        this.mValue = Integer.valueOf(bb.getInt());
    }

    @Override // com.garmin.monkeybrains.serialization.MonkeyType
    public int getNumBytes() {
        return 5;
    }

    @Override // com.garmin.monkeybrains.serialization.MonkeyType
    public byte[] serialize() {
        ByteBuffer bb = ByteBuffer.allocate(getNumBytes());
        bb.put((byte) 19);
        bb.putInt(this.mValue.intValue());
        return bb.array();
    }

    /* JADX WARN: Can't rename method to resolve collision */
    @Override // com.garmin.monkeybrains.serialization.MonkeyType
    public Integer toJava() {
        return Integer.valueOf(this.mValue.intValue());
    }
}
