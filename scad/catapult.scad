use <rackpinion.scad>

rack_teeth = 25;
gear_teeth = 30;
mm_per_tooth = 5;
thickness = 15;
hole = 4;
height = 6;
backboard_thickness = 2;
backboard_height = 2;
side_flange_thickness = 0;
side_flange_height = 10;
stop_height = 2;
left_stop_enable = 1;
right_stop_enable = 1;

length = rack_teeth * mm_per_tooth;
leeway = 2.8;

//gear(mm_per_tooth, gear_teeth, (thickness-backboard_thickness), hole);
//translate([0,40,0])
//    cube([5,50, thickness-backboard_thickness], center=true);
//translate ([50,0,0])
module rack (rail_thickness = 2) {
    union() { // rack
        translate ([length,0,leeway])
            rotate([0,180,0])
                InvoluteGear_rack(mm_per_tooth, rack_teeth, thickness, height, backboard_thickness, backboard_height, side_flange_thickness, side_flange_height, flange_offset, stop_height, left_stop_enable, right_stop_enable);
        InvoluteGear_rack(mm_per_tooth, rack_teeth, thickness, height, backboard_thickness, backboard_height, side_flange_thickness, side_flange_height, flange_offset, stop_height, left_stop_enable, right_stop_enable);
    }
    // rails
    translate ([-height/2,height/2,thickness/2+leeway])
        cube([length+height,rail_thickness,rail_thickness]);
    translate ([-height/2,height/2,-thickness/2-rail_thickness])
        cube([length+height,rail_thickness,rail_thickness]);
}

module track () {
    difference() {
        // shell
        translate ([-height/2, 0, -10-stop_height/2])
            cube([1.5*length, height*3/2, thickness+10]);
        // inner rails
        translate([mm_per_tooth, 0, 0]) // make endstop
            rack(2.5);
        translate([length, 0, 0])
            rack(2.5);
        // remove excess rack negative
        translate ([height/2, -1 ,-thickness/2.5])
            cube([2*length, height*3/2, thickness]);
    }
}


track();
//rack();
