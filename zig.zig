const std = @import("std");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer gpa.deinit();
    const allocator = gpa.allocator();

    var buf: [1024]u8 = undefined;
    const bytes_read = try std.io.getStdIn().read(buf[0..]);
    const input = buf[0..bytes_read];

    std.debug.print("{s}\n", .{input});
}
