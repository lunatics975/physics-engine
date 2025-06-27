#include "world.h"
#include "body.h"
#include "collision_detection.h"
#include "constants.h"
#include "constraint.h"
#include "contact.h"
#include "graphics.h"
#include "vec2.h"
#include <iostream>
#include <vector>

World::World(float gravity) {
    G = -gravity;
}

World::~World() {
    for (auto body : bodies) {
        delete body;
    }
    for (auto constraint : constraints) {
        delete constraint;
    }
}

void World::add_body(Body *body) { bodies.push_back(body); }

std::vector<Body *> &World::get_bodies() { return bodies; }

void World::apply_force(const Vec2 &force) { forces.push_back(force); }
void World::apply_torque(float torque) { torques.push_back(torque); }

/**
 * 1. calculate external forces and acceleration
 * 2. integrate acceleration to find new velocities
 * 3. loop all constraints, solving them
 * 4. integrate velocities to find new positions
 */
void World::update(float dt) {
    // create vector of penetration constraints
    // that will be solved frame per frame
    std::vector<PenetrationConstraint> pens;

    for (auto &body : bodies) {
        Vec2 weight = Vec2(0.0, body->mass * G * PIXELS_PER_METER);
        body->apply_force(weight);

        for (auto force : forces) {
            body->apply_force(force);
        }
        for (auto torque : torques) {
            body->apply_torque(torque);
        }
    }

    // 1. Integrate all forces (a = F/m)
    for (auto &body : bodies) {
        body->integrate_forces(dt);
    }

    for (size_t i = 0; i <= bodies.size() - 1; i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            Body *a = bodies[i];
            Body *b = bodies[j];

            std::vector<Contact> contacts;
            if (CollisionDetection::is_colliding(a, b, contacts)) {
                for (auto contact : contacts) {
                    Graphics::draw_circle(contact.start.x, contact.start.y, 5,
                                          0.0, 0xFF00FFFF);
                    Graphics::draw_circle(contact.end.x, contact.end.y, 2, 0.0,
                                          0xFF00FFFF);
                    // create a new penetration constraint
                    // as soon as collision detected
                    PenetrationConstraint penetration(
                        contact.a, contact.b, contact.start, contact.end,
                        contact.normal);
                    pens.push_back(penetration);
                }
            }
        }
    }

    // 2. solve all constraints
    for (auto &constraint : constraints) {
        constraint->pre_solve(dt);
    }
    for (auto &constraint : pens) {
        constraint.pre_solve(dt);
    }
    for (int i = 0; i < 5; i++) {
        for (auto &constraint : constraints) {
            constraint->solve();
        }
        for (auto &constraint : pens) {
            constraint.solve();
        }
    }

    // 3. integrate velocities (update vertices)
    for (auto &body : bodies) {
        body->integrate_velocities(dt);
    }
}

void World::add_constraint(Constraint *constraint) {
    constraints.push_back(constraint);
}

std::vector<Constraint *> &World::get_constraints() { return constraints; }
