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

module catapult_arm(arm_length = 50){
    gear(mm_per_tooth, gear_teeth, (thickness-backboard_thickness), hole);
    translate([0,arm_length-mm_per_tooth,0])
        cube([5,arm_length, thickness-backboard_thickness], center=true);
}

module basket(length=65, width=45, depth=10, wall_thickness=2) {
    difference () {
        cube ([length,width,depth+wall_thickness]); // outter
        translate([wall_thickness, wall_thickness, wall_thickness])
            cube([length-2*wall_thickness, width-2*wall_thickness, depth]);
    }
    //arm coupling
    translate([length/2, -5, (thickness-backboard_thickness)/2])
        difference() {
            cube([thickness-backboard_thickness+5,10,10], center=true);
            translate([0,-1,0])
            cube([thickness-backboard_thickness+1,10,6], center=true);
        }
}

module reverse_gear() {
    gear(mm_per_tooth, gear_teeth/3, (thickness - backboard_thickness), hole);
}

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

module rack_full() {
    rack();
    handle();
}

module track_base() {
    difference() {
        translate([-height/2, height*5/3, -21-stop_height/2]) 
            cube ([length*1.25, 2, thickness+32]);
        base_holes();
    }
}

module base_holes(dia=4) {
    translate([20,5,20]) rotate([270,0,0]) cylinder(h=20,d=dia);
    translate([20,5,-17]) rotate([270,0,0]) cylinder(h=20,d=dia);
    translate([130,5,20]) rotate([270,0,0]) cylinder(h=20,d=dia);
    translate([130,5,-17]) rotate([270,0,0]) cylinder(h=20,d=dia);
}

module bracket() {
    difference() {
        union() {
            translate([40,-32,11])
                cube([40,42,4]);
            translate([40,-32,-12])
                cube([40,42,4]);
        }
        bracket_holes();
    }
}

module bracket_holes(dia=4.5) {
    translate([50,-6.5+dia/2,0]) // bottom hole
        cylinder(h=50,d=dia,center=true);
    translate([49+26.4-dia,-27.5+dia/2,0]) // top hole
        cylinder(h=50,d=dia,center=true);
        //26.4-dia=x dist between holes
}

module track () {
    difference() {
        // shell
        union() {
            translate ([-height/2, 0, -11-stop_height/2])
                cube([1.25*length, height*5/3, thickness+12]);
            bracket();
            track_base();
        }
        // inner rails
        translate([mm_per_tooth, 0, 0]) // make endstop
            scale ([1.05,1.05,1.05]) rack(2.8);
        translate([length, 0, 0])
            scale ([1.05,1.05,1.05]) rack(2.8);
        // remove excess rack negative
        translate ([height/2-2, -1 ,-thickness/2.5 - 2])
            cube([2.2*length, 1.1*height*3/2, 1.2*thickness+1]);
    }
    track_base();
}

module handle(handle_length = 30, handle_thickness = 5) {
    translate([0,4,leeway/2]) {
        rotate([270,0,30]) {
            cylinder(h=handle_length, d= handle_thickness); 
            translate([0,0,handle_length])
                sphere(d=handle_thickness*2);
        }
    }
}

//track();
basket();
//rack();
//catapult_arm();
//reverse_gear();
